/*
* Copyright (c) 2015-2016 The Khronos Group Inc.
* Copyright (c) 2015-2016 Valve Corporation
* Copyright (c) 2015-2016 LunarG, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Author: Jeremy Hayes <jeremy@lunarg.com>
*/

#if defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
#include <X11/Xutil.h>
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
#include <vulkan/vk_sdk_platform.h>

#include "linmath.h"

#define APP_SHORT_NAME "cube"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#ifdef _WIN32
#define ERR_EXIT(err_msg, err_class)                                           \
    do {                                                                       \
        if (!demo->suppress_popups)                                            \
            MessageBox(NULL, err_msg, err_class, MB_OK);                       \
        exit(1);                                                               \
    } while (0)
#else
#define ERR_EXIT(err_msg, err_class)                                           \
    do {                                                                       \
        printf(err_msg);                                                       \
        fflush(stdout);                                                        \
        exit(1);                                                               \
    } while (0)
#endif

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                               \
    {                                                                          \
        demo->fp##entrypoint =                                                 \
            (PFN_vk##entrypoint)vkGetInstanceProcAddr(inst, "vk" #entrypoint); \
        if (demo->fp##entrypoint == NULL) {                                    \
            ERR_EXIT("vkGetInstanceProcAddr failed to find vk" #entrypoint,    \
                     "vkGetInstanceProcAddr Failure");                         \
        }                                                                      \
    }

static PFN_vkGetDeviceProcAddr g_gdpa = NULL;

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                  \
    {                                                                          \
        if (!g_gdpa)                                                           \
            g_gdpa = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(           \
                demo->inst, "vkGetDeviceProcAddr");                            \
        demo->fp##entrypoint =                                                 \
            (PFN_vk##entrypoint)g_gdpa(dev, "vk" #entrypoint);                 \
        if (demo->fp##entrypoint == NULL) {                                    \
            ERR_EXIT("vkGetDeviceProcAddr failed to find vk" #entrypoint,      \
                     "vkGetDeviceProcAddr Failure");                           \
        }                                                                      \
    }

struct texture_object {
    vk::Sampler sampler;

    vk::Image image;
    vk::ImageLayout imageLayout;

    vk::MemoryAllocateInfo mem_alloc;
    vk::DeviceMemory mem;
    vk::ImageView view;

    int32_t tex_width;
    int32_t tex_height;
};

static char const*const tex_files[] = {"lunarg.ppm"};

static int validation_error = 0;

struct vkcube_vs_uniform {
    // Must start with MVP
    float mvp[4][4];
    float position[12 * 3][4];
    float color[12 * 3][4];
};

struct vktexcube_vs_uniform {
    // Must start with MVP
    float mvp[4][4];
    float position[12 * 3][4];
    float attr[12 * 3][4];
};

//--------------------------------------------------------------------------------------
// Mesh and VertexFormat Data
//--------------------------------------------------------------------------------------
// clang-format off
static const float g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f,  // -X side
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,

    -1.0f,-1.0f,-1.0f,  // -Z side
     1.0f, 1.0f,-1.0f,
     1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,

    -1.0f,-1.0f,-1.0f,  // -Y side
     1.0f,-1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
     1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,

    -1.0f, 1.0f,-1.0f,  // +Y side
    -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,

     1.0f, 1.0f,-1.0f,  // +X side
     1.0f, 1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,
     1.0f, 1.0f,-1.0f,

    -1.0f, 1.0f, 1.0f,  // +Z side
    -1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
};

static const float g_uv_buffer_data[] = {
    0.0f, 1.0f,  // -X side
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,

    1.0f, 1.0f,  // -Z side
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,  // -Y side
    1.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,  // +Y side
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    1.0f, 0.0f,  // +X side
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 0.0f,  // +Z side
    0.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

typedef struct
{
    vk::Image image;
    vk::CommandBuffer cmd;
    vk::ImageView view;
} SwapchainBuffers;

struct Demo
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
#define APP_NAME_STR_LEN 80
    HINSTANCE connection;        // hInstance - Windows Instance
    char name[APP_NAME_STR_LEN]; // Name to put on the window/icon
    HWND window;                 // hWnd - window handle
    POINT minsize;               // minimum window size
#elif defined(VK_USE_PLATFORM_XLIB_KHR) | defined(VK_USE_PLATFORM_XCB_KHR)
    Display* display;
    Window xlib_window;
    Atom xlib_wm_delete_window;

    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t xcb_window;
    xcb_intern_atom_reply_t *atom_wm_delete_window;
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct wl_surface *window;
    struct wl_shell *shell;
    struct wl_shell_surface *shell_surface;
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    ANativeWindow* window;
#endif

    vk::SurfaceKHR surface;
    bool prepared;
    bool use_staging_buffer;
    bool use_xlib;

    vk::Instance inst;
    vk::PhysicalDevice gpu;
    vk::Device device;
    vk::Queue graphics_queue;
    vk::Queue present_queue;
    uint32_t graphics_queue_family_index;
    uint32_t present_queue_family_index;
    vk::Semaphore image_acquired_semaphore;
    vk::Semaphore draw_complete_semaphore;
    vk::PhysicalDeviceProperties gpu_props;
    vk::QueueFamilyProperties *queue_props;
    vk::PhysicalDeviceMemoryProperties memory_properties;

    uint32_t enabled_extension_count;
    uint32_t enabled_layer_count;
    char const* extension_names[64];
    char const* enabled_layers[64];

    int width, height;
    vk::Format format;
    vk::ColorSpaceKHR color_space;

    PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
    PFN_vkQueuePresentKHR fpQueuePresentKHR;
    uint32_t swapchainImageCount;
    vk::SwapchainKHR swapchain;
    SwapchainBuffers *buffers;

    vk::CommandPool cmd_pool;

    struct
    {
        vk::Format format;
        vk::Image image;
        vk::MemoryAllocateInfo mem_alloc;
        vk::DeviceMemory mem;
        vk::ImageView view;
    } depth;

    static int32_t const texture_count = 1;
    struct texture_object textures[texture_count];

    struct
    {
        vk::Buffer buf;
        vk::MemoryAllocateInfo mem_alloc;
        vk::DeviceMemory mem;
        vk::DescriptorBufferInfo buffer_info;
    } uniform_data;

    vk::CommandBuffer cmd; // Buffer for initialization commands
    vk::PipelineLayout pipeline_layout;
    vk::DescriptorSetLayout desc_layout;
    vk::PipelineCache pipelineCache;
    vk::RenderPass render_pass;
    vk::Pipeline pipeline;

    mat4x4 projection_matrix;
    mat4x4 view_matrix;
    mat4x4 model_matrix;

    float spin_angle;
    float spin_increment;
    bool pause;

    vk::ShaderModule vert_shader_module;
    vk::ShaderModule frag_shader_module;

    vk::DescriptorPool desc_pool;
    vk::DescriptorSet desc_set;

    vk::Framebuffer *framebuffers;

    bool quit;
    int32_t curFrame;
    int32_t frameCount;
    bool validate;
    bool use_break;
    bool suppress_popups;
    PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback;
    PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback;
    vk::DebugReportCallbackEXT msg_callback;
    PFN_vkDebugReportMessageEXT DebugReportMessage;

    uint32_t current_buffer;
    uint32_t queue_count;
};

VKAPI_ATTR VkBool32 VKAPI_CALL
BreakCallback(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
              uint64_t srcObject, size_t location, int32_t msgCode,
              const char *pLayerPrefix, const char *pMsg,
              void *pUserData) {
#ifndef WIN32
    raise(SIGTRAP);
#else
    DebugBreak();
#endif

    return false;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
dbgFunc(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
    uint64_t srcObject, size_t location, int32_t msgCode,
    const char *pLayerPrefix, const char *pMsg, void *pUserData) {
    char *message = (char *)malloc(strlen(pMsg) + 100);

    assert(message);

    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        sprintf(message, "ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode,
            pMsg);
        validation_error = 1;
    } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        // We know that we're submitting queues without fences, ignore this
        // warning
        if (strstr(pMsg,
            "vkQueueSubmit parameter, VkFence fence, is null pointer")) {
            return false;
        }
        sprintf(message, "WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode,
            pMsg);
        validation_error = 1;
    } else {
        validation_error = 1;
        return false;
    }

#ifdef _WIN32
    Demo *demo = (Demo*) pUserData;
    if (!demo->suppress_popups)
        MessageBox(NULL, message, "Alert", MB_OK);
#else
    printf("%s\n", message);
    fflush(stdout);
#endif
    free(message);

    /*
    * false indicates that layer should not bail-out of an
    * API call that had validation failures. This may mean that the
    * app dies inside the driver due to invalid parameter(s).
    * That's what would happen without validation layers, so we'll
    * keep that behavior here.
    */
    return false;
}

static vk::Bool32 demo_check_layers(uint32_t check_count, char const*const*const check_names, uint32_t layer_count,
    vk::LayerProperties *layers);
static void demo_cleanup(Demo *demo);
static void demo_create_device(Demo *demo);
static void demo_destroy_texture_image(Demo *demo, struct texture_object *tex_objs);
static void demo_draw(Demo *demo);
static void demo_draw_build_cmd(Demo *demo, vk::CommandBuffer commandBuffer);
static void demo_flush_init_cmd(Demo *demo);
static void demo_init(Demo *demo, int argc, char **argv);
static void demo_init_connection(Demo *demo);
static void demo_init_vk(Demo *demo);
static void demo_init_vk_swapchain(Demo *demo);
static void demo_prepare(Demo *demo);
static void demo_prepare_buffers(Demo *demo);
static void demo_prepare_cube_data_buffer(Demo *demo);
static void demo_prepare_depth(Demo *demo);
static void demo_prepare_descriptor_layout(Demo *demo);
static void demo_prepare_descriptor_pool(Demo *demo);
static void demo_prepare_descriptor_set(Demo *demo);
static void demo_prepare_framebuffers(Demo *demo);
static VkShaderModule demo_prepare_fs(Demo *demo);
static void demo_prepare_pipeline(Demo *demo);
static void demo_prepare_render_pass(Demo *demo);
static VkShaderModule demo_prepare_shader_module(Demo *demo, const void *code, size_t size);
static void demo_prepare_texture_image(Demo *demo, const char *filename,
                                       struct texture_object *tex_obj,
                                       VkImageTiling tiling,
                                       VkImageUsageFlags usage,
                                       VkFlags required_props);
static void demo_prepare_textures(Demo *demo);
static VkShaderModule demo_prepare_vs(Demo *demo);
static char *demo_read_spv(const char *filename, size_t *psize);
static void demo_resize(Demo *demo);
static void demo_set_image_layout(Demo *demo, VkImage image,
                                  VkImageAspectFlags aspectMask,
                                  VkImageLayout old_image_layout,
                                  VkImageLayout new_image_layout,
                                  VkAccessFlagBits srcAccessMask);
static void demo_update_data_buffer(Demo *demo);
static bool loadTexture(const char *filename, uint8_t *rgba_data,
                 VkSubresourceLayout *layout, int32_t *width, int32_t *height);
static bool memory_type_from_properties(Demo *demo, uint32_t typeBits,
                                        VkFlags requirements_mask,
                                        uint32_t *typeIndex);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
static void demo_run(Demo *demo);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void demo_create_window(Demo *demo);
#elif defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
#if defined(VK_USE_PLATFORM_XLIB_KHR)
static void demo_create_xlib_window(Demo *demo);
static void demo_handle_xlib_event(Demo *demo, const XEvent *event);
static void demo_run_xlib(Demo *demo);
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
static void demo_handle_xcb_event(Demo *demo,
	const xcb_generic_event_t *event);
static void demo_run_xcb(Demo *demo);
static void demo_create_xcb_window(Demo *demo);
#endif
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
static void demo_run(Demo *demo);
static void demo_create_window(Demo *demo);
static void handle_ping(void *data UNUSED,
                        struct wl_shell_surface *shell_surface,
                        uint32_t serial);
static void handle_configure(void *data UNUSED,
                             struct wl_shell_surface *shell_surface UNUSED,
                             uint32_t edges UNUSED, int32_t width UNUSED,
                             int32_t height UNUSED);
static void handle_popup_done(void *data UNUSED,
                              struct wl_shell_surface *shell_surface UNUSED);

static const struct wl_shell_surface_listener shell_surface_listener =
{
    handle_ping,
	handle_configure,
	handle_popup_done
};
#endif

static vk::Bool32 demo_check_layers(uint32_t check_count, char const*const*const check_names, uint32_t layer_count,
    vk::LayerProperties *layers)
{
    for (uint32_t i = 0; i < check_count; i++) {
        VkBool32 found = 0;
        for (uint32_t j = 0; j < layer_count; j++) {
            if (!strcmp(check_names[i], layers[j].layerName)) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Cannot find layer: %s\n", check_names[i]);
            return 0;
        }
    }
    return 1;
}

static void demo_cleanup(Demo *demo)
{
    demo->prepared = false;
    demo->device.waitIdle();

    for(uint32_t i = 0; i < demo->swapchainImageCount; i++)
    {
        demo->device.destroyFramebuffer(demo->framebuffers[i], NULL);
    }
    free(demo->framebuffers);
    demo->device.destroyDescriptorPool(demo->desc_pool, NULL);

    demo->device.destroyPipeline(demo->pipeline, NULL);
    demo->device.destroyPipelineCache(demo->pipelineCache, NULL);
    demo->device.destroyRenderPass(demo->render_pass, NULL);
    demo->device.destroyPipelineLayout(demo->pipeline_layout, NULL);
    demo->device.destroyDescriptorSetLayout(demo->desc_layout, NULL);

    for(uint32_t i = 0; i < demo->texture_count; i++)
    {
        demo->device.destroyImageView(demo->textures[i].view, NULL);
        demo->device.destroyImage(demo->textures[i].image, NULL);
        demo->device.freeMemory(demo->textures[i].mem, NULL);
        demo->device.destroySampler(demo->textures[i].sampler, NULL);
    }
    demo->device.destroySwapchainKHR(demo->swapchain, NULL);

    demo->device.destroyImageView(demo->depth.view, NULL);
    demo->device.destroyImage(demo->depth.image, NULL);
    demo->device.freeMemory(demo->depth.mem, NULL);

    demo->device.destroyBuffer(demo->uniform_data.buf, NULL);
    demo->device.freeMemory(demo->uniform_data.mem, NULL);

    for(uint32_t i = 0; i < demo->swapchainImageCount; i++)
    {
        demo->device.destroyImageView(demo->buffers[i].view, NULL);
        demo->device.freeCommandBuffers(demo->cmd_pool, 1, &demo->buffers[i].cmd);
    }
    free(demo->buffers);

    free(demo->queue_props);

    demo->device.destroyCommandPool(demo->cmd_pool, NULL);
    demo->device.destroySemaphore(demo->image_acquired_semaphore, NULL);
    demo->device.destroySemaphore(demo->draw_complete_semaphore, NULL);
    demo->device.destroy(NULL);
    if (demo->validate)
    {
        demo->inst.destroyDebugReportCallbackEXT(demo->msg_callback, NULL);
    }
    demo->inst.destroySurfaceKHR(demo->surface, NULL);
    demo->inst.destroy(NULL);

// TODO: What if both xlib and xcb are enabled?
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (demo->use_xlib) {
        XDestroyWindow(demo->display, demo->xlib_window);
        XCloseDisplay(demo->display);
    } else {
        xcb_destroy_window(demo->connection, demo->xcb_window);
        xcb_disconnect(demo->connection);
    }
    free(demo->atom_wm_delete_window);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    xcb_destroy_window(demo->connection, demo->xcb_window);
    xcb_disconnect(demo->connection);
    free(demo->atom_wm_delete_window);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    wl_shell_surface_destroy(demo->shell_surface);
    wl_surface_destroy(demo->window);
    wl_shell_destroy(demo->shell);
    wl_compositor_destroy(demo->compositor);
    wl_registry_destroy(demo->registry);
    wl_display_disconnect(demo->display);
#endif
}

static void demo_create_device(Demo *demo)
{
    float const priorities[1] = {0.0};

    auto const queueInfo = vk::DeviceQueueCreateInfo()
        .setQueueFamilyIndex(demo->graphics_queue_family_index)
        .setQueueCount(1)
        .setPQueuePriorities(priorities);

    auto const deviceInfo = vk::DeviceCreateInfo()
        .setQueueCreateInfoCount(1)
        .setPQueueCreateInfos(&queueInfo)
        .setEnabledLayerCount(0)
        .setPpEnabledLayerNames(nullptr)
        .setEnabledExtensionCount(demo->enabled_extension_count)
        .setPpEnabledExtensionNames((const char *const *)demo->extension_names)
        .setPEnabledFeatures(nullptr);

    vk::Result result = demo->gpu.createDevice(&deviceInfo, nullptr, &demo->device);
    assert(result == vk::Result::eSuccess);
}

static void demo_destroy_texture_image(Demo *demo, struct texture_object *tex_objs)
{
    // clean up staging resources
    demo->device.freeMemory(tex_objs->mem, nullptr);
    demo->device.destroyImage(tex_objs->image, nullptr);
}

static void demo_draw(Demo *demo) {
    // Get the index of the next available swapchain image:
    vk::Result result = demo->device.acquireNextImageKHR(demo->swapchain, UINT64_MAX, demo->image_acquired_semaphore, (VkFence)0,
        &demo->current_buffer);
    if(result == vk::Result::eErrorOutOfDateKHR)
    {
        // demo->swapchain is out of date (e.g. the window was resized) and
        // must be recreated:
        demo_resize(demo);
        demo_draw(demo);
        return;
    }
    else if(result == vk::Result::eSuboptimalKHR)
    {
        // demo->swapchain is not as optimal as it could be, but the platform's
        // presentation engine will still present the image correctly.
    }
    else
    {
        assert(result == vk::Result::eSuccess);
    }

    // Wait for the image acquired semaphore to be signaled to ensure
    // that the image won't be rendered to until the presentation
    // engine has fully released ownership to the application, and it is
    // okay to render to the image.
    vk::PipelineStageFlags const pipelineStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    auto const submitInfo = vk::SubmitInfo()
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&demo->image_acquired_semaphore)
        .setPWaitDstStageMask(&pipelineStageFlags)
        .setCommandBufferCount(1)
        .setPCommandBuffers(&demo->buffers[demo->current_buffer].cmd)
        .setSignalSemaphoreCount(1)
        .setPSignalSemaphores(&demo->draw_complete_semaphore);

    result = demo->graphics_queue.submit(1, &submitInfo, vk::Fence());
    assert(result == vk::Result::eSuccess);

    auto const presentInfo = vk::PresentInfoKHR()
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&demo->draw_complete_semaphore)
        .setSwapchainCount(1)
        .setPSwapchains(&demo->swapchain)
        .setPImageIndices(&demo->current_buffer);

    result = demo->present_queue.presentKHR(&presentInfo);
    if(result == vk::Result::eErrorOutOfDateKHR)
    {
        // demo->swapchain is out of date (e.g. the window was resized) and
        // must be recreated:
        demo_resize(demo);
    }
    else if(result == vk::Result::eSuboptimalKHR)
    {
        // demo->swapchain is not as optimal as it could be, but the platform's
        // presentation engine will still present the image correctly.
    }
    else
    {
        assert(result == vk::Result::eSuccess);
    }
}

static void demo_draw_build_cmd(Demo *demo, vk::CommandBuffer commandBuffer)
{
    auto const commandInfo = vk::CommandBufferBeginInfo()
        .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

    vk::ClearValue const clearValues[2] =
    {
        vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })),
	vk::ClearDepthStencilValue(1.0f, 0u)
    };

    auto const passInfo = vk::RenderPassBeginInfo()
        .setRenderPass(demo->render_pass)
        .setFramebuffer(demo->framebuffers[demo->current_buffer])
        .setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D((uint32_t)demo->width, (uint32_t)demo->height)))
        .setClearValueCount(2)
        .setPClearValues(clearValues);

    vk::Result result = commandBuffer.begin(&commandInfo);
    assert(result == vk::Result::eSuccess);

    // We can use LAYOUT_UNDEFINED as a wildcard here because we don't care what
    // happens to the previous contents of the image
    auto const image_memory_barrier = vk::ImageMemoryBarrier()
        .setSrcAccessMask(vk::AccessFlagBits(0))
        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
        .setOldLayout(vk::ImageLayout::eUndefined)
        .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
        .setImage(demo->buffers[demo->current_buffer].image)
        .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::DependencyFlagBits(0),
        0, NULL,
        0, NULL,
        1, &image_memory_barrier);

    commandBuffer.beginRenderPass(&passInfo, vk::SubpassContents::eInline);

    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, demo->pipeline);
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, demo->pipeline_layout,
        0, 1, &demo->desc_set,
        0, NULL);

    auto const viewport = vk::Viewport()
        .setWidth((float)demo->width)
        .setHeight((float)demo->height)
        .setMinDepth((float)0.0f)
        .setMaxDepth((float)1.0f);
    commandBuffer.setViewport(0, 1, &viewport);

    vk::Rect2D const scissor(vk::Offset2D(0, 0), vk::Extent2D(demo->width, demo->height));
    commandBuffer.setScissor(0, 1, &scissor);
    commandBuffer.draw(12 * 3, 1, 0, 0);
    // Note that ending the renderpass changes the image's layout from
    // COLOR_ATTACHEMENT_OPTIMAL to PRESENT_SRC_KHR
    commandBuffer.endRenderPass();

    result = commandBuffer.end();
    assert(result == vk::Result::eSuccess);
}

static void demo_flush_init_cmd(Demo *demo) {
    // TODO: Following comment makes me cringe.
    // This function could get called twice if the texture uses a staging buffer
    // In that case the second call should be ignored
    if (demo->cmd == vk::CommandBuffer(VK_NULL_HANDLE))
    {
        return;
    }

    vk::Result result = demo->cmd.end();
    assert(result == vk::Result::eSuccess);

    auto const fenceInfo = vk::FenceCreateInfo()
        .setFlags(vk::FenceCreateFlagBits(0));
    vk::Fence fence;
    demo->device.createFence(&fenceInfo, NULL, &fence);

    vk::CommandBuffer const commandBuffers[] =
    {
        demo->cmd
    };
    auto const submitInfo = vk::SubmitInfo()
        .setCommandBufferCount(1)
        .setPCommandBuffers(commandBuffers);

    result = demo->graphics_queue.submit(1, &submitInfo, fence);
    assert(result == vk::Result::eSuccess);

    result = demo->device.waitForFences(1, &fence, VK_TRUE, UINT64_MAX);
    assert(result == vk::Result::eSuccess);

    demo->device.freeCommandBuffers(demo->cmd_pool, 1, commandBuffers);
    demo->device.destroyFence(fence, nullptr);

    demo->cmd = vk::CommandBuffer(VK_NULL_HANDLE);
}

static void demo_init(Demo *demo, int argc, char **argv)
{
    vec3 eye = {0.0f, 3.0f, 5.0f};
    vec3 origin = {0, 0, 0};
    vec3 up = {0.0f, 1.0f, 0.0};

    memset(demo, 0, sizeof(*demo));
    demo->frameCount = INT32_MAX;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--use_staging") == 0) {
            demo->use_staging_buffer = true;
            continue;
        }
        if (strcmp(argv[i], "--break") == 0) {
            demo->use_break = true;
            continue;
        }
        if (strcmp(argv[i], "--validate") == 0) {
            demo->validate = true;
            continue;
        }
#if defined(VK_USE_PLATFORM_XLIB_KHR)
        if (strcmp(argv[i], "--xlib") == 0) {
            demo->use_xlib = true;
            continue;
        }
#endif
        if (strcmp(argv[i], "--c") == 0 && demo->frameCount == INT32_MAX &&
            i < argc - 1 && sscanf(argv[i + 1], "%d", &demo->frameCount) == 1 &&
            demo->frameCount >= 0) {
            i++;
            continue;
        }
        if (strcmp(argv[i], "--suppress_popups") == 0) {
            demo->suppress_popups = true;
            continue;
        }

        fprintf(stderr, "Usage:\n  %s [--use_staging] [--validate] [--break] "
#if defined(VK_USE_PLATFORM_XLIB_KHR)
                        "[--xlib] "
#endif
                        "[--c <framecount>] [--suppress_popups]\n",
                APP_SHORT_NAME);
        fflush(stderr);
        exit(1);
    }

    if (!demo->use_xlib)
        demo_init_connection(demo);

    demo_init_vk(demo);

    demo->width = 500;
    demo->height = 500;

    demo->spin_angle = 0.01f;
    demo->spin_increment = 0.01f;
    demo->pause = false;

    mat4x4_perspective(demo->projection_matrix, (float)degreesToRadians(45.0f),
                       1.0f, 0.1f, 100.0f);
    mat4x4_look_at(demo->view_matrix, eye, origin, up);
    mat4x4_identity(demo->model_matrix);

    demo->projection_matrix[1][1]*=-1;  //Flip projection matrix from GL to Vulkan orientation.
}

static void demo_init_connection(Demo *demo) {
#if defined(VK_USE_PLATFORM_XCB_KHR)
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    demo->connection = xcb_connect(NULL, &scr);
    if (xcb_connection_has_error(demo->connection) > 0) {
        printf("Cannot find a compatible Vulkan installable client driver "
               "(ICD).\nExiting ...\n");
        fflush(stdout);
        exit(1);
    }

    setup = xcb_get_setup(demo->connection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0)
        xcb_screen_next(&iter);

    demo->screen = iter.data;
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    demo->display = wl_display_connect(NULL);

    if (demo->display == NULL) {
        printf("Cannot find a compatible Vulkan installable client driver "
               "(ICD).\nExiting ...\n");
        fflush(stdout);
        exit(1);
    }

    demo->registry = wl_display_get_registry(demo->display);
    wl_registry_add_listener(demo->registry, &registry_listener, demo);
    wl_display_dispatch(demo->display);
#endif
}

static void demo_init_vk(Demo *demo) {
    VkResult err;
    uint32_t instance_extension_count = 0;
    uint32_t instance_layer_count = 0;
    uint32_t validation_layer_count = 0;
    char const*const* instance_validation_layers = NULL;
    demo->enabled_extension_count = 0;
    demo->enabled_layer_count = 0;

    char const*const instance_validation_layers_alt1[] = {
        "VK_LAYER_LUNARG_standard_validation"
    };

    char const*const instance_validation_layers_alt2[] = {
        "VK_LAYER_GOOGLE_threading",       "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker",  "VK_LAYER_LUNARG_image",
        "VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_GOOGLE_unique_objects"
    };

    /* Look for validation layers */
    VkBool32 validation_found = 0;
    if (demo->validate) {

        err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
        assert(!err);

        instance_validation_layers = instance_validation_layers_alt1;
        if (instance_layer_count > 0) {
            VkLayerProperties *instance_layers = (VkLayerProperties*)
                    malloc(sizeof (VkLayerProperties) * instance_layer_count);
            err = vkEnumerateInstanceLayerProperties(&instance_layer_count,
                    instance_layers);
            assert(!err);


            validation_found = demo_check_layers(
                    ARRAY_SIZE(instance_validation_layers_alt1),
                    instance_validation_layers, instance_layer_count,
                    instance_layers);
            if (validation_found) {
                demo->enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt1);
                demo->enabled_layers[0] = "VK_LAYER_LUNARG_standard_validation";
                validation_layer_count = 1;
            } else {
                // use alternative set of validation layers
                instance_validation_layers = instance_validation_layers_alt2;
                demo->enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt2);
                validation_found = demo_check_layers(
                    ARRAY_SIZE(instance_validation_layers_alt2),
                    instance_validation_layers, instance_layer_count,
                    instance_layers);
                validation_layer_count =
                    ARRAY_SIZE(instance_validation_layers_alt2);
                for (uint32_t i = 0; i < validation_layer_count; i++) {
                    demo->enabled_layers[i] = instance_validation_layers[i];
                }
            }
            free(instance_layers);
        }

        if (!validation_found) {
            ERR_EXIT("vkEnumerateInstanceLayerProperties failed to find "
                    "required validation layer.\n\n"
                    "Please look at the Getting Started guide for additional "
                    "information.\n",
                    "vkCreateInstance Failure");
        }
    }

    /* Look for instance extensions */
    VkBool32 surfaceExtFound = 0;
    VkBool32 platformSurfaceExtFound = 0;
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    VkBool32 xlibSurfaceExtFound = 0;
#endif
    memset(demo->extension_names, 0, sizeof(demo->extension_names));

    err = vkEnumerateInstanceExtensionProperties(
        NULL, &instance_extension_count, NULL);
    assert(!err);

    if (instance_extension_count > 0) {
        VkExtensionProperties *instance_extensions = (VkExtensionProperties*)
            malloc(sizeof(VkExtensionProperties) * instance_extension_count);
        err = vkEnumerateInstanceExtensionProperties(
            NULL, &instance_extension_count, instance_extensions);
        assert(!err);
        for (uint32_t i = 0; i < instance_extension_count; i++) {
            if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                surfaceExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_SURFACE_EXTENSION_NAME;
            }
#if defined(VK_USE_PLATFORM_WIN32_KHR)
            if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                platformSurfaceExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
            }
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
            if (!strcmp(VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                platformSurfaceExtFound = 1;
                xlibSurfaceExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
            }
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
            if (!strcmp(VK_KHR_XCB_SURFACE_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                platformSurfaceExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_XCB_SURFACE_EXTENSION_NAME;
            }
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
            if (!strcmp(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                platformSurfaceExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
            }
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
            if (!strcmp(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                platformSurfaceExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
            }
#endif
            if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                        instance_extensions[i].extensionName)) {
                if (demo->validate) {
                    demo->extension_names[demo->enabled_extension_count++] =
                        VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
                }
            }
            assert(demo->enabled_extension_count < 64);
        }

        free(instance_extensions);
    }

    if (!surfaceExtFound) {
        ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                 "the " VK_KHR_SURFACE_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
    }
    if (!platformSurfaceExtFound) {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
        ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                 "the " VK_KHR_WIN32_SURFACE_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_XCB_KHR)
        ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                 "the " VK_KHR_XCB_SURFACE_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
        ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                 "the " VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
        ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                 "the " VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
#endif
    }
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    if (demo->use_xlib && !xlibSurfaceExtFound) {
        ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                 "the " VK_KHR_XLIB_SURFACE_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
    }
#endif
    const VkApplicationInfo app = {
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        NULL,
        APP_SHORT_NAME,
        0,
        APP_SHORT_NAME,
        0,
        VK_API_VERSION_1_0,
    };
    VkInstanceCreateInfo inst_info = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        NULL,
		0,
        &app,
        demo->enabled_layer_count,
        instance_validation_layers,
        demo->enabled_extension_count,
        demo->extension_names,
    };

    /*
     * This is info for a temp callback to use during CreateInstance.
     * After the instance is created, we use the instance-based
     * function to register the final callback.
     */
    VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
    if (demo->validate) {
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        dbgCreateInfo.pNext = NULL;
        dbgCreateInfo.pfnCallback = demo->use_break ? BreakCallback : dbgFunc;
        dbgCreateInfo.pUserData = demo;
        dbgCreateInfo.flags =
            VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        inst_info.pNext = &dbgCreateInfo;
    }

    uint32_t gpu_count;

    err = vkCreateInstance(&inst_info, NULL, &demo->inst);
    if (err == VK_ERROR_INCOMPATIBLE_DRIVER) {
        ERR_EXIT("Cannot find a compatible Vulkan installable client driver "
                 "(ICD).\n\nPlease look at the Getting Started guide for "
                 "additional information.\n",
                 "vkCreateInstance Failure");
    } else if (err == VK_ERROR_EXTENSION_NOT_PRESENT) {
        ERR_EXIT("Cannot find a specified extension library"
                 ".\nMake sure your layers path is set appropriately.\n",
                 "vkCreateInstance Failure");
    } else if (err) {
        ERR_EXIT("vkCreateInstance failed.\n\nDo you have a compatible Vulkan "
                 "installable client driver (ICD) installed?\nPlease look at "
                 "the Getting Started guide for additional information.\n",
                 "vkCreateInstance Failure");
    }

    /* Make initial call to query gpu_count, then second call for gpu info*/
    err = vkEnumeratePhysicalDevices(demo->inst, &gpu_count, NULL);
    assert(!err && gpu_count > 0);

    if (gpu_count > 0) {
        VkPhysicalDevice *physical_devices = (VkPhysicalDevice*) malloc(sizeof(VkPhysicalDevice) * gpu_count);
        err = vkEnumeratePhysicalDevices(demo->inst, &gpu_count, physical_devices);
        assert(!err);
        /* For cube demo we just grab the first physical device */
        demo->gpu = physical_devices[0];
        free(physical_devices);
    } else {
        ERR_EXIT("vkEnumeratePhysicalDevices reported zero accessible devices.\n\n"
                 "Do you have a compatible Vulkan installable client driver (ICD) "
                 "installed?\nPlease look at the Getting Started guide for "
                 "additional information.\n",
                 "vkEnumeratePhysicalDevices Failure");
    }

    /* Look for device extensions */
    uint32_t device_extension_count = 0;
    VkBool32 swapchainExtFound = 0;
    demo->enabled_extension_count = 0;
    memset(demo->extension_names, 0, sizeof(demo->extension_names));

    err = vkEnumerateDeviceExtensionProperties(demo->gpu, NULL,
                                               &device_extension_count, NULL);
    assert(!err);

    if (device_extension_count > 0) {
        VkExtensionProperties *device_extensions = (VkExtensionProperties*)
            malloc(sizeof(VkExtensionProperties) * device_extension_count);
        err = vkEnumerateDeviceExtensionProperties(
            demo->gpu, NULL, &device_extension_count, device_extensions);
        assert(!err);

        for (uint32_t i = 0; i < device_extension_count; i++) {
            if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                        device_extensions[i].extensionName)) {
                swapchainExtFound = 1;
                demo->extension_names[demo->enabled_extension_count++] =
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME;
            }
            assert(demo->enabled_extension_count < 64);
        }

        free(device_extensions);
    }

    if (!swapchainExtFound) {
        ERR_EXIT("vkEnumerateDeviceExtensionProperties failed to find "
                 "the " VK_KHR_SWAPCHAIN_EXTENSION_NAME
                 " extension.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n",
                 "vkCreateInstance Failure");
    }

    if (demo->validate) {
        demo->CreateDebugReportCallback =
            (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
                demo->inst, "vkCreateDebugReportCallbackEXT");
        demo->DestroyDebugReportCallback =
            (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
                demo->inst, "vkDestroyDebugReportCallbackEXT");
        if (!demo->CreateDebugReportCallback) {
            ERR_EXIT(
                "GetProcAddr: Unable to find vkCreateDebugReportCallbackEXT\n",
                "vkGetProcAddr Failure");
        }
        if (!demo->DestroyDebugReportCallback) {
            ERR_EXIT(
                "GetProcAddr: Unable to find vkDestroyDebugReportCallbackEXT\n",
                "vkGetProcAddr Failure");
        }
        demo->DebugReportMessage =
            (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(
                demo->inst, "vkDebugReportMessageEXT");
        if (!demo->DebugReportMessage) {
            ERR_EXIT("GetProcAddr: Unable to find vkDebugReportMessageEXT\n",
                     "vkGetProcAddr Failure");
        }

        VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
        PFN_vkDebugReportCallbackEXT callback;
        callback = demo->use_break ? BreakCallback : dbgFunc;
        dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        dbgCreateInfo.pNext = NULL;
        dbgCreateInfo.pfnCallback = callback;
        dbgCreateInfo.pUserData = demo;
        dbgCreateInfo.flags =
            VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        err = demo->CreateDebugReportCallback(demo->inst, &dbgCreateInfo, NULL,
                                              &demo->msg_callback);
        switch (err) {
        case VK_SUCCESS:
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            ERR_EXIT("CreateDebugReportCallback: out of host memory\n",
                     "CreateDebugReportCallback Failure");
            break;
        default:
            ERR_EXIT("CreateDebugReportCallback: unknown failure\n",
                     "CreateDebugReportCallback Failure");
            break;
        }
    }
    vkGetPhysicalDeviceProperties(demo->gpu, &demo->gpu_props);

    /* Call with NULL data to get count */
    vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queue_count,
                                             NULL);
    assert(demo->queue_count >= 1);

    demo->queue_props = (VkQueueFamilyProperties *)malloc(
        demo->queue_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queue_count,
                                             demo->queue_props);
    // Find a queue that supports gfx
    uint32_t gfx_queue_idx = 0;
    for (gfx_queue_idx = 0; gfx_queue_idx < demo->queue_count;
         gfx_queue_idx++) {
        if (demo->queue_props[gfx_queue_idx].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            break;
    }
    assert(gfx_queue_idx < demo->queue_count);
    // Query fine-grained feature support for this device.
    //  If app has specific feature requirements it should check supported
    //  features based on this query
    VkPhysicalDeviceFeatures physDevFeatures;
    vkGetPhysicalDeviceFeatures(demo->gpu, &physDevFeatures);

    GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceSupportKHR);
    GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceCapabilitiesKHR);
    GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceFormatsKHR);
    GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfacePresentModesKHR);
    GET_INSTANCE_PROC_ADDR(demo->inst, GetSwapchainImagesKHR);
}

static void demo_init_vk_swapchain(Demo *demo) {
// Create a WSI surface for the window:
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.hinstance = demo->connection;
    createInfo.hwnd = demo->window;

    VkResult err = vkCreateWin32SurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
    assert(!err);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR) && !defined(VK_USE_PLATFORM_XCB_KHR)
    VkWaylandSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.display = demo->display;
    createInfo.surface = demo->window;

    err = vkCreateWaylandSurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
    assert(!err);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    VkAndroidSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.window = (ANativeWindow*)(demo->window);

    err = vkCreateAndroidSurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
    assert(!err);
#endif
    if (demo->use_xlib) {
#if defined(VK_USE_PLATFORM_XLIB_KHR)
        VkXlibSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.dpy = demo->display;
        createInfo.window = demo->xlib_window;

        VkResult err = vkCreateXlibSurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
        assert(!err);
#endif
    }
    else {
#if defined(VK_USE_PLATFORM_XCB_KHR)
        VkXcbSurfaceCreateInfoKHR createInfo;
        createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = NULL;
        createInfo.flags = 0;
        createInfo.connection = demo->connection;
        createInfo.window = demo->xcb_window;

        VkResult err = vkCreateXcbSurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
        assert(!err);
#endif
    }

    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *supportsPresent =
        (VkBool32 *)malloc(demo->queue_count * sizeof(VkBool32));
    for (uint32_t i = 0; i < demo->queue_count; i++) {
        demo->fpGetPhysicalDeviceSurfaceSupportKHR(demo->gpu, i, demo->surface,
                                                   &supportsPresent[i]);
    }

    // Search for a graphics and a present queue in the array of queue
    // families, try to find one that supports both
    uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    uint32_t presentQueueNodeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < demo->queue_count; i++) {
        if ((demo->queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0
                && graphicsQueueNodeIndex == UINT32_MAX) {
            graphicsQueueNodeIndex = i;
        }

        if (supportsPresent[i] == VK_TRUE && presentQueueNodeIndex == UINT32_MAX) {
            presentQueueNodeIndex = i;
        }
    }

    // Generate error if could not find both a graphics and a present queue
    if (graphicsQueueNodeIndex == UINT32_MAX ||
        presentQueueNodeIndex == UINT32_MAX) {
        ERR_EXIT("Could not find both graphics and present queues\n",
                 "Swapchain Initialization Failure");
    }

    demo->graphics_queue_family_index = graphicsQueueNodeIndex;
    demo->present_queue_family_index = presentQueueNodeIndex;

    demo_create_device(demo);

    GET_DEVICE_PROC_ADDR(demo->device, CreateSwapchainKHR);
    GET_DEVICE_PROC_ADDR(demo->device, DestroySwapchainKHR);
    GET_DEVICE_PROC_ADDR(demo->device, GetSwapchainImagesKHR);
    GET_DEVICE_PROC_ADDR(demo->device, AcquireNextImageKHR);
    GET_DEVICE_PROC_ADDR(demo->device, QueuePresentKHR);

    vkGetDeviceQueue(demo->device, demo->graphics_queue_family_index, 0,
                     &demo->graphics_queue);

    if (demo->graphics_queue_family_index == demo->present_queue_family_index) {
        demo->present_queue = demo->graphics_queue;
    } else {
        vkGetDeviceQueue(demo->device, demo->present_queue_family_index, 0,
                         &demo->present_queue);
    }

    // Get the list of VkFormat's that are supported:
    uint32_t formatCount;
    VkResult err = demo->fpGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface,
                                                     &formatCount, NULL);
    assert(!err);
    VkSurfaceFormatKHR *surfFormats =
        (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    err = demo->fpGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface,
                                                     &formatCount, surfFormats);
    assert(!err);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        demo->format = VK_FORMAT_B8G8R8A8_UNORM;
    } else {
        assert(formatCount >= 1);
        demo->format = surfFormats[0].format;
    }
    demo->color_space = surfFormats[0].colorSpace;

    demo->quit = false;
    demo->curFrame = 0;

    // Create semaphores to synchronize acquiring presentable buffers before
    // rendering and waiting for drawing to be complete before presenting
    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
    };
    err = vkCreateSemaphore(demo->device, &semaphoreCreateInfo, NULL,
                            &demo->image_acquired_semaphore);
    assert(!err);

    err = vkCreateSemaphore(demo->device, &semaphoreCreateInfo, NULL,
                            &demo->draw_complete_semaphore);
    assert(!err);

    // Get Memory information and properties
    vkGetPhysicalDeviceMemoryProperties(demo->gpu, &demo->memory_properties);
}

static void demo_prepare(Demo *demo) {
    const VkCommandPoolCreateInfo cmd_pool_info = {
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        NULL,
        0,
        demo->graphics_queue_family_index
    };
    VkResult err = vkCreateCommandPool(demo->device, &cmd_pool_info, NULL,
                              &demo->cmd_pool);
    assert(!err);

    const VkCommandBufferAllocateInfo cmd = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = demo->cmd_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    demo_prepare_buffers(demo);
    demo_prepare_depth(demo);
    demo_prepare_textures(demo);
    demo_prepare_cube_data_buffer(demo);

    demo_prepare_descriptor_layout(demo);
    demo_prepare_render_pass(demo);
    demo_prepare_pipeline(demo);

    for (uint32_t i = 0; i < demo->swapchainImageCount; i++) {
        err =
            vkAllocateCommandBuffers(demo->device, &cmd, &demo->buffers[i].cmd);
        assert(!err);
    }

    demo_prepare_descriptor_pool(demo);
    demo_prepare_descriptor_set(demo);

    demo_prepare_framebuffers(demo);

    for (uint32_t i = 0; i < demo->swapchainImageCount; i++) {
        demo->current_buffer = i;
        demo_draw_build_cmd(demo, demo->buffers[i].cmd);
    }

    /*
     * Prepare functions above may generate pipeline commands
     * that need to be flushed before beginning the render loop.
     */
    demo_flush_init_cmd(demo);

    demo->current_buffer = 0;
    demo->prepared = true;
}

static void demo_prepare_buffers(Demo *demo) {
    VkSwapchainKHR oldSwapchain = demo->swapchain;

    // Check the surface capabilities and formats
    VkSurfaceCapabilitiesKHR surfCapabilities;
    VkResult err = demo->fpGetPhysicalDeviceSurfaceCapabilitiesKHR(
        demo->gpu, demo->surface, &surfCapabilities);
    assert(!err);

    uint32_t presentModeCount;
    err = demo->fpGetPhysicalDeviceSurfacePresentModesKHR(
        demo->gpu, demo->surface, &presentModeCount, NULL);
    assert(!err);
    VkPresentModeKHR *presentModes =
        (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
    assert(presentModes);
    err = demo->fpGetPhysicalDeviceSurfacePresentModesKHR(
        demo->gpu, demo->surface, &presentModeCount, presentModes);
    assert(!err);

    VkExtent2D swapchainExtent;
    // width and height are either both -1, or both not -1.
    if (surfCapabilities.currentExtent.width == (uint32_t)-1) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = demo->width;
        swapchainExtent.height = demo->height;
    } else {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
        demo->width = surfCapabilities.currentExtent.width;
        demo->height = surfCapabilities.currentExtent.height;
    }

    // If mailbox mode is available, use it, as is the lowest-latency non-
    // tearing mode.  If not, try IMMEDIATE which will usually be available,
    // and is fastest (though it tears).  If not, fall back to FIFO which is
    // always available.
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (size_t i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
            (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
            swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
    }

    // Determine the number of VkImage's to use in the swap chain (we desire to
    // own only 1 image at a time, besides the images being displayed and
    // queued for display):
    uint32_t desiredNumberOfSwapchainImages =
        surfCapabilities.minImageCount + 1;
    // If maxImageCount is 0, we can ask for as many images as we want, otherwise
    // we're limited to maxImageCount
    if ((surfCapabilities.maxImageCount > 0) &&
        (desiredNumberOfSwapchainImages > surfCapabilities.maxImageCount)) {
        // Application must settle for fewer images than desired:
        desiredNumberOfSwapchainImages = surfCapabilities.maxImageCount;
    }

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms &
        VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        preTransform = surfCapabilities.currentTransform;
    }

    VkSwapchainCreateInfoKHR swapchain_ci = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        NULL,
		0,
        demo->surface,
        desiredNumberOfSwapchainImages,
        demo->format,
        demo->color_space,
        { swapchainExtent.width, swapchainExtent.height },
        1,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        NULL,
        preTransform,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        swapchainPresentMode,
        true,
        oldSwapchain
    };
    uint32_t i;
    uint32_t queueFamilyIndices[2] = {(uint32_t) demo->graphics_queue_family_index, (uint32_t) demo->present_queue_family_index};
    if (demo->graphics_queue_family_index != demo->present_queue_family_index)
    {
        // If the graphics and present queues are from different queue families, we either have to
        // explicitly transfer ownership of images between the queues, or we have to create the swapchain
        // with imageSharingMode as VK_SHARING_MODE_CONCURRENT
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }

    err = demo->fpCreateSwapchainKHR(demo->device, &swapchain_ci, NULL,
                                     &demo->swapchain);
    assert(!err);

    // If we just re-created an existing swapchain, we should destroy the old
    // swapchain at this point.
    // Note: destroying the swapchain also cleans up all its associated
    // presentable images once the platform is done with them.
    if (oldSwapchain != VK_NULL_HANDLE) {
        demo->fpDestroySwapchainKHR(demo->device, oldSwapchain, NULL);
    }

    err = demo->fpGetSwapchainImagesKHR(demo->device, demo->swapchain,
                                        &demo->swapchainImageCount, NULL);
    assert(!err);

    VkImage *swapchainImages =
        (VkImage *)malloc(demo->swapchainImageCount * sizeof(VkImage));
    assert(swapchainImages);
    err = demo->fpGetSwapchainImagesKHR(demo->device, demo->swapchain,
                                        &demo->swapchainImageCount,
                                        swapchainImages);
    assert(!err);

    demo->buffers = (SwapchainBuffers *)malloc(sizeof(SwapchainBuffers) *
                                               demo->swapchainImageCount);
    assert(demo->buffers);

    for (i = 0; i < demo->swapchainImageCount; i++) {
        VkImageViewCreateInfo color_image_view = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            NULL,
            0,
			VK_NULL_HANDLE,
            VK_IMAGE_VIEW_TYPE_2D,
            demo->format,
            {
                 VK_COMPONENT_SWIZZLE_R,
                 VK_COMPONENT_SWIZZLE_G,
                 VK_COMPONENT_SWIZZLE_B,
                 VK_COMPONENT_SWIZZLE_A,
			},
            {
				VK_IMAGE_ASPECT_COLOR_BIT,
				0,
				1,
				0,
				1
			}
        };

        demo->buffers[i].image = swapchainImages[i];

        color_image_view.image = demo->buffers[i].image;

        err = vkCreateImageView(demo->device, &color_image_view, NULL,
                                &demo->buffers[i].view);
        assert(!err);
    }


    if (NULL != presentModes) {
        free(presentModes);
    }
}

static void demo_prepare_cube_data_buffer(Demo *demo) {
    VkBufferCreateInfo buf_info;
    VkMemoryRequirements mem_reqs;
    uint8_t *pData;
    int i;
    mat4x4 MVP, VP;
    struct vktexcube_vs_uniform data;

    mat4x4_mul(VP, demo->projection_matrix, demo->view_matrix);
    mat4x4_mul(MVP, VP, demo->model_matrix);
    memcpy(data.mvp, MVP, sizeof(MVP));
    //    dumpMatrix("MVP", MVP);

    for (i = 0; i < 12 * 3; i++) {
        data.position[i][0] = g_vertex_buffer_data[i * 3];
        data.position[i][1] = g_vertex_buffer_data[i * 3 + 1];
        data.position[i][2] = g_vertex_buffer_data[i * 3 + 2];
        data.position[i][3] = 1.0f;
        data.attr[i][0] = g_uv_buffer_data[2 * i];
        data.attr[i][1] = g_uv_buffer_data[2 * i + 1];
        data.attr[i][2] = 0;
        data.attr[i][3] = 0;
    }

    memset(&buf_info, 0, sizeof(buf_info));
    buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    buf_info.size = sizeof(data);
    VkResult err =
        vkCreateBuffer(demo->device, &buf_info, NULL, &demo->uniform_data.buf);
    assert(!err);

    vkGetBufferMemoryRequirements(demo->device, demo->uniform_data.buf,
                                  &mem_reqs);

    demo->uniform_data.mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    demo->uniform_data.mem_alloc.pNext = NULL;
    demo->uniform_data.mem_alloc.allocationSize = mem_reqs.size;
    demo->uniform_data.mem_alloc.memoryTypeIndex = 0;

    bool pass = memory_type_from_properties(
        demo, mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &demo->uniform_data.mem_alloc.memoryTypeIndex);
    assert(pass);

    err = vkAllocateMemory(demo->device, &demo->uniform_data.mem_alloc, NULL,
                           &(demo->uniform_data.mem));
    assert(!err);

    err = vkMapMemory(demo->device, demo->uniform_data.mem, 0,
                      demo->uniform_data.mem_alloc.allocationSize, 0,
                      (void **)&pData);
    assert(!err);

    memcpy(pData, &data, sizeof data);

    vkUnmapMemory(demo->device, demo->uniform_data.mem);

    err = vkBindBufferMemory(demo->device, demo->uniform_data.buf,
                             demo->uniform_data.mem, 0);
    assert(!err);

    demo->uniform_data.buffer_info.buffer = demo->uniform_data.buf;
    demo->uniform_data.buffer_info.offset = 0;
    demo->uniform_data.buffer_info.range = sizeof(data);
}

static void demo_prepare_depth(Demo *demo) {
    const VkFormat depth_format = VK_FORMAT_D16_UNORM;
    const VkImageCreateInfo image = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        NULL,
        0,
        VK_IMAGE_TYPE_2D,
        depth_format,
        { (uint32_t)demo->width, (uint32_t)demo->height, 1},
        1,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr,
		VK_IMAGE_LAYOUT_UNDEFINED
    };

    VkImageViewCreateInfo view = {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        NULL,
        0,
        VK_NULL_HANDLE,
        VK_IMAGE_VIEW_TYPE_2D,
        depth_format,
		{},
        {
			VK_IMAGE_ASPECT_DEPTH_BIT,
            0,
			1,
            0,
            1
		}
    };

    VkMemoryRequirements mem_reqs;

    demo->depth.format = depth_format;

    /* create image */
    VkResult err = vkCreateImage(demo->device, &image, NULL, &demo->depth.image);
    assert(!err);

    vkGetImageMemoryRequirements(demo->device, demo->depth.image, &mem_reqs);
    assert(!err);

    demo->depth.mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    demo->depth.mem_alloc.pNext = NULL;
    demo->depth.mem_alloc.allocationSize = mem_reqs.size;
    demo->depth.mem_alloc.memoryTypeIndex = 0;

    bool pass = memory_type_from_properties(demo, mem_reqs.memoryTypeBits,
                                       0, /* No requirements */
                                       &demo->depth.mem_alloc.memoryTypeIndex);
    assert(pass);

    /* allocate memory */
    err = vkAllocateMemory(demo->device, &demo->depth.mem_alloc, NULL,
                           &demo->depth.mem);
    assert(!err);

    /* bind memory */
    err =
        vkBindImageMemory(demo->device, demo->depth.image, demo->depth.mem, 0);
    assert(!err);

    demo_set_image_layout(demo, demo->depth.image, VK_IMAGE_ASPECT_DEPTH_BIT,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                          (VkAccessFlagBits)0);

    /* create image view */
    view.image = demo->depth.image;
    err = vkCreateImageView(demo->device, &view, NULL, &demo->depth.view);
    assert(!err);
}

static void demo_prepare_descriptor_layout(Demo *demo) {
    const VkDescriptorSetLayoutBinding layout_bindings[2] = {
            [0] =
                {
                 .binding = 0,
                 .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                 .descriptorCount = 1,
                 .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                 .pImmutableSamplers = NULL,
                },
            [1] =
                {
                 .binding = 1,
                 .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                 .descriptorCount = demo->texture_count,
                 .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                 .pImmutableSamplers = NULL,
                },
    };
    const VkDescriptorSetLayoutCreateInfo descriptor_layout = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        NULL,
		0,
        2,
        layout_bindings,
    };

    VkResult err = vkCreateDescriptorSetLayout(demo->device, &descriptor_layout, NULL,
                                      &demo->desc_layout);
    assert(!err);

    const VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        NULL,
		0,
        1,
        &demo->desc_layout,
		0,
		nullptr
    };

    err = vkCreatePipelineLayout(demo->device, &pPipelineLayoutCreateInfo, NULL,
                                 &demo->pipeline_layout);
    assert(!err);
}

static void demo_prepare_descriptor_pool(Demo *demo) {
    const VkDescriptorPoolSize type_counts[2] = {
            [0] =
                {
                 .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                 .descriptorCount = 1,
                },
            [1] =
                {
                 .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                 .descriptorCount = demo->texture_count,
                },
    };
    const VkDescriptorPoolCreateInfo descriptor_pool = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        NULL,
		0,
        1,
        2,
        type_counts
    };

    VkResult err = vkCreateDescriptorPool(demo->device, &descriptor_pool, NULL,
                                 &demo->desc_pool);
    assert(!err);
}

static void demo_prepare_descriptor_set(Demo *demo) {
    VkDescriptorImageInfo tex_descs[demo->texture_count];
    VkWriteDescriptorSet writes[2];

    VkDescriptorSetAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = NULL,
        .descriptorPool = demo->desc_pool,
        .descriptorSetCount = 1,
        .pSetLayouts = &demo->desc_layout};
    VkResult err = vkAllocateDescriptorSets(demo->device, &alloc_info, &demo->desc_set);
    assert(!err);

    memset(&tex_descs, 0, sizeof(tex_descs));
    for (uint32_t i = 0; i < demo->texture_count; i++) {
        tex_descs[i].sampler = demo->textures[i].sampler;
        tex_descs[i].imageView = demo->textures[i].view;
        tex_descs[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    }

    memset(&writes, 0, sizeof(writes));

    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstSet = demo->desc_set;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    writes[0].pBufferInfo = &demo->uniform_data.buffer_info;

    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet = demo->desc_set;
    writes[1].dstBinding = 1;
    writes[1].descriptorCount = demo->texture_count;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[1].pImageInfo = tex_descs;

    vkUpdateDescriptorSets(demo->device, 2, writes, 0, NULL);
}

static void demo_prepare_framebuffers(Demo *demo) {
    VkImageView attachments[2];
    attachments[1] = demo->depth.view;

    const VkFramebufferCreateInfo fb_info = {
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        NULL,
		0,
        demo->render_pass,
        2,
        attachments,
        (uint32_t)demo->width,
        (uint32_t)demo->height,
        1
    };

    demo->framebuffers = (VkFramebuffer *)malloc(demo->swapchainImageCount *
                                                 sizeof(VkFramebuffer));
    assert(demo->framebuffers);

    for (uint32_t i = 0; i < demo->swapchainImageCount; i++) {
        attachments[0] = demo->buffers[i].view;
        VkResult err = vkCreateFramebuffer(demo->device, &fb_info, NULL,
                                  &demo->framebuffers[i]);
        assert(!err);
    }
}

static VkShaderModule demo_prepare_fs(Demo *demo) {
    void *fragShaderCode;
    size_t size;

    fragShaderCode = demo_read_spv("cube-frag.spv", &size);

    demo->frag_shader_module =
        demo_prepare_shader_module(demo, fragShaderCode, size);

    free(fragShaderCode);

    return demo->frag_shader_module;
}

static void demo_prepare_pipeline(Demo *demo) {
    VkGraphicsPipelineCreateInfo pipeline;
    VkPipelineCacheCreateInfo pipelineCache;
    VkPipelineVertexInputStateCreateInfo vi;
    VkPipelineInputAssemblyStateCreateInfo ia;
    VkPipelineRasterizationStateCreateInfo rs;
    VkPipelineColorBlendStateCreateInfo cb;
    VkPipelineDepthStencilStateCreateInfo ds;
    VkPipelineViewportStateCreateInfo vp;
    VkPipelineMultisampleStateCreateInfo ms;
    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    VkPipelineDynamicStateCreateInfo dynamicState;

    memset(dynamicStateEnables, 0, sizeof dynamicStateEnables);
    memset(&dynamicState, 0, sizeof dynamicState);
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pDynamicStates = dynamicStateEnables;

    memset(&pipeline, 0, sizeof(pipeline));
    pipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline.layout = demo->pipeline_layout;

    memset(&vi, 0, sizeof(vi));
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    memset(&ia, 0, sizeof(ia));
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    memset(&rs, 0, sizeof(rs));
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rs.depthClampEnable = VK_FALSE;
    rs.rasterizerDiscardEnable = VK_FALSE;
    rs.depthBiasEnable = VK_FALSE;
    rs.lineWidth = 1.0f;

    memset(&cb, 0, sizeof(cb));
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    VkPipelineColorBlendAttachmentState att_state[1];
    memset(att_state, 0, sizeof(att_state));
    att_state[0].colorWriteMask = 0xf;
    att_state[0].blendEnable = VK_FALSE;
    cb.attachmentCount = 1;
    cb.pAttachments = att_state;

    memset(&vp, 0, sizeof(vp));
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.viewportCount = 1;
    dynamicStateEnables[dynamicState.dynamicStateCount++] =
        VK_DYNAMIC_STATE_VIEWPORT;
    vp.scissorCount = 1;
    dynamicStateEnables[dynamicState.dynamicStateCount++] =
        VK_DYNAMIC_STATE_SCISSOR;

    memset(&ds, 0, sizeof(ds));
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.depthTestEnable = VK_TRUE;
    ds.depthWriteEnable = VK_TRUE;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    ds.depthBoundsTestEnable = VK_FALSE;
    ds.back.failOp = VK_STENCIL_OP_KEEP;
    ds.back.passOp = VK_STENCIL_OP_KEEP;
    ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
    ds.stencilTestEnable = VK_FALSE;
    ds.front = ds.back;

    memset(&ms, 0, sizeof(ms));
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pSampleMask = NULL;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Two stages: vs and fs
    pipeline.stageCount = 2;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    memset(&shaderStages, 0, 2 * sizeof(VkPipelineShaderStageCreateInfo));

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = demo_prepare_vs(demo);
    shaderStages[0].pName = "main";

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = demo_prepare_fs(demo);
    shaderStages[1].pName = "main";

    memset(&pipelineCache, 0, sizeof(pipelineCache));
    pipelineCache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    VkResult err = vkCreatePipelineCache(demo->device, &pipelineCache, NULL,
                                &demo->pipelineCache);
    assert(!err);

    pipeline.pVertexInputState = &vi;
    pipeline.pInputAssemblyState = &ia;
    pipeline.pRasterizationState = &rs;
    pipeline.pColorBlendState = &cb;
    pipeline.pMultisampleState = &ms;
    pipeline.pViewportState = &vp;
    pipeline.pDepthStencilState = &ds;
    pipeline.pStages = shaderStages;
    pipeline.renderPass = demo->render_pass;
    pipeline.pDynamicState = &dynamicState;

    pipeline.renderPass = demo->render_pass;

    err = vkCreateGraphicsPipelines(demo->device, demo->pipelineCache, 1,
                                    &pipeline, NULL, &demo->pipeline);
    assert(!err);

    vkDestroyShaderModule(demo->device, demo->frag_shader_module, NULL);
    vkDestroyShaderModule(demo->device, demo->vert_shader_module, NULL);
}

static void demo_prepare_render_pass(Demo *demo) {
    const VkAttachmentDescription attachments[2] =
	{
		{
			0,
			demo->format,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		},
		{
			0,
			demo->depth.format,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		}
    };

    const VkAttachmentReference color_reference =
	{
        0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    const VkAttachmentReference depth_reference =
	{
        1,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    };

    const VkSubpassDescription subpass =
	{
        0,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        0,
        NULL,
        1,
        &color_reference,
        NULL,
        &depth_reference,
        0,
        NULL,
    };

    const VkRenderPassCreateInfo rp_info =
	{
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        NULL,
		0,
        2,
        attachments,
        1,
        &subpass,
        0,
        NULL,
    };

    VkResult err = vkCreateRenderPass(demo->device, &rp_info, NULL, &demo->render_pass);
    assert(!err);
}

static VkShaderModule demo_prepare_shader_module(Demo *demo, const void *code, size_t size) {
    VkShaderModule module;
    VkShaderModuleCreateInfo moduleCreateInfo;

    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = NULL;

    moduleCreateInfo.codeSize = size;
    moduleCreateInfo.pCode = (uint32_t const*) code;
    moduleCreateInfo.flags = 0;

    VkResult err = vkCreateShaderModule(demo->device, &moduleCreateInfo, NULL, &module);
    assert(!err);

    return module;
}

static void demo_prepare_texture_image(Demo *demo, const char *filename,
                                       struct texture_object *tex_obj,
                                       VkImageTiling tiling,
                                       VkImageUsageFlags usage,
                                       VkFlags required_props) {
    const VkFormat tex_format = VK_FORMAT_R8G8B8A8_UNORM;
    int32_t tex_width;
    int32_t tex_height;

    if (!loadTexture(filename, NULL, NULL, &tex_width, &tex_height)) {
        ERR_EXIT("Failed to load textures", "Load Texture Failure");
    }

    tex_obj->tex_width = tex_width;
    tex_obj->tex_height = tex_height;

    const VkImageCreateInfo image_create_info = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        NULL,
        0,
        VK_IMAGE_TYPE_2D,
        tex_format,
        {(uint32_t)tex_width, (uint32_t)tex_height, 1},
        1,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        tiling,
        usage,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		nullptr,
        VK_IMAGE_LAYOUT_PREINITIALIZED
    };

    VkMemoryRequirements mem_reqs;

    vk::Result result = demo->device.createImage(&image_create_info, NULL, &tex_obj->image);
    assert(result == vk::Result::eSuccess);

    vkGetImageMemoryRequirements(demo->device, tex_obj->image, &mem_reqs);

    tex_obj->mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    tex_obj->mem_alloc.pNext = NULL;
    tex_obj->mem_alloc.allocationSize = mem_reqs.size;
    tex_obj->mem_alloc.memoryTypeIndex = 0;

    bool pass = memory_type_from_properties(demo, mem_reqs.memoryTypeBits,
                                       required_props,
                                       &tex_obj->mem_alloc.memoryTypeIndex);
    assert(pass);

    /* allocate memory */
    result = vkAllocateMemory(demo->device, &tex_obj->mem_alloc, NULL,
                           &(tex_obj->mem));
    assert(result == vk::Result::eSuccess);

    /* bind memory */
    result = vkBindImageMemory(demo->device, tex_obj->image, tex_obj->mem, 0);
    assert(result == vk::Result::eSuccess);

    if (required_props & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
        const VkImageSubresource subres = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .arrayLayer = 0,
        };
        VkSubresourceLayout layout;
        vkGetImageSubresourceLayout(demo->device, tex_obj->image, &subres,
                                    &layout);

        void *data;
        result = vkMapMemory(demo->device, tex_obj->mem, 0,
                          tex_obj->mem_alloc.allocationSize, 0, &data);
        assert(result == vk::Result::eSuccess);

        if (!loadTexture(filename, (uint8_t*)data, &layout, &tex_width, &tex_height)) {
            fprintf(stderr, "Error loading texture: %s\n", filename);
        }

        vkUnmapMemory(demo->device, tex_obj->mem);
    }

    tex_obj->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    demo_set_image_layout(demo, tex_obj->image, VK_IMAGE_ASPECT_COLOR_BIT,
                          VK_IMAGE_LAYOUT_PREINITIALIZED, tex_obj->imageLayout,
                          VK_ACCESS_HOST_WRITE_BIT);
    /* setting the image layout does not reference the actual memory so no need
     * to add a mem ref */
}

static void demo_prepare_textures(Demo *demo) {
    const VkFormat tex_format = VK_FORMAT_R8G8B8A8_UNORM;
    VkFormatProperties props;
    uint32_t i;

    vkGetPhysicalDeviceFormatProperties(demo->gpu, tex_format, &props);

    for (i = 0; i < demo->texture_count; i++) {
        if ((props.linearTilingFeatures &
             VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) &&
            !demo->use_staging_buffer) {
            /* Device can texture using linear textures */
            demo_prepare_texture_image(
                demo, tex_files[i], &demo->textures[i], VK_IMAGE_TILING_LINEAR,
                VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        } else if (props.optimalTilingFeatures &
                   VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) {
            /* Must use staging buffer to copy linear texture to optimized */
            struct texture_object staging_texture;

            memset(&staging_texture, 0, sizeof(staging_texture));
            demo_prepare_texture_image(
                demo, tex_files[i], &staging_texture, VK_IMAGE_TILING_LINEAR,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            demo_prepare_texture_image(
                demo, tex_files[i], &demo->textures[i], VK_IMAGE_TILING_OPTIMAL,
                (VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            demo_set_image_layout(demo, staging_texture.image,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  staging_texture.imageLayout,
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  (VkAccessFlagBits)0);

            demo_set_image_layout(demo, demo->textures[i].image,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  demo->textures[i].imageLayout,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  (VkAccessFlagBits)0);

            VkImageCopy copy_region = {
                .srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
                .srcOffset = {0, 0, 0},
                .dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1},
                .dstOffset = {0, 0, 0},
                .extent = {(uint32_t)staging_texture.tex_width, (uint32_t)staging_texture.tex_height, 1},
            };
            vkCmdCopyImage(
                demo->cmd, staging_texture.image,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, demo->textures[i].image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);

            demo_set_image_layout(demo, demo->textures[i].image,
                                  VK_IMAGE_ASPECT_COLOR_BIT,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  demo->textures[i].imageLayout,
                                  (VkAccessFlagBits)0);

            demo_flush_init_cmd(demo);

            demo_destroy_texture_image(demo, &staging_texture);
        } else {
            /* Can't support VK_FORMAT_R8G8B8A8_UNORM !? */
            assert(!"No support for R8G8B8A8_UNORM as texture image format");
        }

        const VkSamplerCreateInfo sampler = {
            VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            NULL,
			0,
            VK_FILTER_NEAREST,
            VK_FILTER_NEAREST,
            VK_SAMPLER_MIPMAP_MODE_NEAREST,
            VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            0.0f,
            VK_FALSE,
            1,
			VK_FALSE,
            VK_COMPARE_OP_NEVER,
            0.0f,
            0.0f,
            VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
            VK_FALSE,
        };

        VkImageViewCreateInfo view = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            NULL,
            0,
            VK_NULL_HANDLE,
            VK_IMAGE_VIEW_TYPE_2D,
            tex_format,
            {
                VK_COMPONENT_SWIZZLE_R,
                VK_COMPONENT_SWIZZLE_G,
                VK_COMPONENT_SWIZZLE_B,
                VK_COMPONENT_SWIZZLE_A,
            },
            {
                VK_IMAGE_ASPECT_COLOR_BIT,
                0,
                1,
                0,
                1
            }
        };

        /* create sampler */
        VkResult err = vkCreateSampler(demo->device, &sampler, NULL,
                              &demo->textures[i].sampler);
        assert(!err);

        /* create image view */
        view.image = demo->textures[i].image;
        err = vkCreateImageView(demo->device, &view, NULL,
                                &demo->textures[i].view);
        assert(!err);
    }
}

static VkShaderModule demo_prepare_vs(Demo *demo) {
    void *vertShaderCode;
    size_t size;

    vertShaderCode = demo_read_spv("cube-vert.spv", &size);

    demo->vert_shader_module =
        demo_prepare_shader_module(demo, vertShaderCode, size);

    free(vertShaderCode);

    return demo->vert_shader_module;
}

static char *demo_read_spv(const char *filename, size_t *psize) {
    long int size;
    void *shader_code;

    FILE *fp = fopen(filename, "rb");
    if (!fp)
        return NULL;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    shader_code = malloc(size);
    size_t retval = fread(shader_code, size, 1, fp);
    assert(retval == 1);

    *psize = size;

    fclose(fp);
    return (char*)shader_code;
}

static void demo_resize(Demo *demo) {
    uint32_t i;

    // Don't react to resize until after first initialization.
    if (!demo->prepared) {
        return;
    }
    // In order to properly resize the window, we must re-create the swapchain
    // AND redo the command buffers, etc.
    //
    // First, perform part of the demo_cleanup() function:
    demo->prepared = false;
    vkDeviceWaitIdle(demo->device);

    for (i = 0; i < demo->swapchainImageCount; i++) {
        vkDestroyFramebuffer(demo->device, demo->framebuffers[i], NULL);
    }
    free(demo->framebuffers);
    vkDestroyDescriptorPool(demo->device, demo->desc_pool, NULL);

    vkDestroyPipeline(demo->device, demo->pipeline, NULL);
    vkDestroyPipelineCache(demo->device, demo->pipelineCache, NULL);
    vkDestroyRenderPass(demo->device, demo->render_pass, NULL);
    vkDestroyPipelineLayout(demo->device, demo->pipeline_layout, NULL);
    vkDestroyDescriptorSetLayout(demo->device, demo->desc_layout, NULL);

    for (i = 0; i < demo->texture_count; i++) {
        vkDestroyImageView(demo->device, demo->textures[i].view, NULL);
        vkDestroyImage(demo->device, demo->textures[i].image, NULL);
        vkFreeMemory(demo->device, demo->textures[i].mem, NULL);
        vkDestroySampler(demo->device, demo->textures[i].sampler, NULL);
    }

    vkDestroyImageView(demo->device, demo->depth.view, NULL);
    vkDestroyImage(demo->device, demo->depth.image, NULL);
    vkFreeMemory(demo->device, demo->depth.mem, NULL);

    vkDestroyBuffer(demo->device, demo->uniform_data.buf, NULL);
    vkFreeMemory(demo->device, demo->uniform_data.mem, NULL);

    for (i = 0; i < demo->swapchainImageCount; i++) {
        vkDestroyImageView(demo->device, demo->buffers[i].view, NULL);
        vkFreeCommandBuffers(demo->device, demo->cmd_pool, 1,
                             &demo->buffers[i].cmd);
    }
    vkDestroyCommandPool(demo->device, demo->cmd_pool, NULL);
    free(demo->buffers);

    // Second, re-perform the demo_prepare() function, which will re-create the
    // swapchain:
    demo_prepare(demo);
}

static void demo_set_image_layout(Demo *demo, VkImage image,
                                  VkImageAspectFlags aspectMask,
                                  VkImageLayout old_image_layout,
                                  VkImageLayout new_image_layout,
                                  VkAccessFlagBits srcAccessMask) {
    if (demo->cmd == VK_NULL_HANDLE) {
        const VkCommandBufferAllocateInfo cmd = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = NULL,
            .commandPool = demo->cmd_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkResult err = vkAllocateCommandBuffers(demo->device, &cmd, &demo->cmd);
        assert(!err);
        VkCommandBufferBeginInfo cmd_buf_info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = NULL,
            .flags = 0,
            .pInheritanceInfo = NULL,
        };
        err = vkBeginCommandBuffer(demo->cmd, &cmd_buf_info);
        assert(!err);
    }

    VkImageMemoryBarrier image_memory_barrier =
    {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        NULL,
        srcAccessMask,
        0,
        old_image_layout,
        new_image_layout,
        0,
        0,
        image,
        {
            aspectMask,
            0,
            1,
            0,
            1
        }
    };

    if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        /* Make sure anything that was copying from this image has completed */
        image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

    if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        /* Make sure any Copy or CPU writes to image are flushed */
        image_memory_barrier.dstAccessMask =
            VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    }

    VkImageMemoryBarrier *pmemory_barrier = &image_memory_barrier;

    VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(demo->cmd, src_stages, dest_stages, 0, 0, NULL, 0,
                         NULL, 1, pmemory_barrier);
}

static void demo_update_data_buffer(Demo *demo) {
    mat4x4 MVP, Model, VP;
    int matrixSize = sizeof(MVP);
    uint8_t *pData;

    mat4x4_mul(VP, demo->projection_matrix, demo->view_matrix);

    // Rotate 22.5 degrees around the Y axis
    mat4x4_dup(Model, demo->model_matrix);
    mat4x4_rotate(demo->model_matrix, Model, 0.0f, 1.0f, 0.0f,
                  (float)degreesToRadians(demo->spin_angle));
    mat4x4_mul(MVP, VP, demo->model_matrix);

    VkResult err = vkMapMemory(demo->device, demo->uniform_data.mem, 0,
                      demo->uniform_data.mem_alloc.allocationSize, 0,
                      (void **)&pData);
    assert(!err);

    memcpy(pData, (const void *)&MVP[0][0], matrixSize);

    vkUnmapMemory(demo->device, demo->uniform_data.mem);
}

static bool loadTexture(const char *filename, uint8_t *rgba_data,
                 VkSubresourceLayout *layout, int32_t *width, int32_t *height) {
    FILE *fPtr = fopen(filename, "rb");
    char header[256], *cPtr, *tmp;

    if (!fPtr)
        return false;

    cPtr = fgets(header, 256, fPtr); // P6
    if (cPtr == NULL || strncmp(header, "P6\n", 3)) {
        fclose(fPtr);
        return false;
    }

    do {
        cPtr = fgets(header, 256, fPtr);
        if (cPtr == NULL) {
            fclose(fPtr);
            return false;
        }
    } while (!strncmp(header, "#", 1));

    sscanf(header, "%u %u", width, height);
    if (rgba_data == NULL) {
        fclose(fPtr);
        return true;
    }
    tmp = fgets(header, 256, fPtr); // Format
    (void)tmp;
    if (cPtr == NULL || strncmp(header, "255\n", 3)) {
        fclose(fPtr);
        return false;
    }

    for (int y = 0; y < *height; y++) {
        uint8_t *rowPtr = rgba_data;
        for (int x = 0; x < *width; x++) {
            size_t s = fread(rowPtr, 3, 1, fPtr);
            (void)s;
            rowPtr[3] = 255; /* Alpha of 1 */
            rowPtr += 4;
        }
        rgba_data += layout->rowPitch;
    }
    fclose(fPtr);
    return true;
}

static bool memory_type_from_properties(Demo *demo, uint32_t typeBits,
                                        VkFlags requirements_mask,
                                        uint32_t *typeIndex) {
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
        if ((typeBits & 1) == 1) {
            // Type is available, does it match user properties?
            if ((demo->memory_properties.memoryTypes[i].propertyFlags &
                 requirements_mask) == requirements_mask) {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    // No memory types matched, return failure
    return false;
}

#if defined(VK_USE_PLATFORM_WIN32_KHR)
static void demo_run(Demo *demo) {
    if (!demo->prepared)
        return;

    demo_update_data_buffer(demo);
    demo_draw(demo);
    demo->curFrame++;
    if (demo->frameCount != INT_MAX && demo->curFrame == demo->frameCount) {
        PostQuitMessage(validation_error);
    }
}

// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(validation_error);
        break;
    case WM_PAINT:
        demo_run(&demo);
        break;
    case WM_GETMINMAXINFO:     // set window's minimum size
        ((MINMAXINFO*)lParam)->ptMinTrackSize = demo.minsize;
        return 0;
    case WM_SIZE:
        // Resize the application to the new window size, except when
        // it was minimized. Vulkan doesn't support images or swapchains
        // with width=0 and height=0.
        if (wParam != SIZE_MINIMIZED) {
            demo.width = lParam & 0xffff;
            demo.height = (lParam & 0xffff0000) >> 16;
            demo_resize(&demo);
        }
        break;
    default:
        break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

static void demo_create_window(Demo *demo) {
    WNDCLASSEX win_class;

    // Initialize the window class structure:
    win_class.cbSize = sizeof(WNDCLASSEX);
    win_class.style = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc = WndProc;
    win_class.cbClsExtra = 0;
    win_class.cbWndExtra = 0;
    win_class.hInstance = demo->connection; // hInstance
    win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    win_class.lpszMenuName = NULL;
    win_class.lpszClassName = demo->name;
    win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    if (!RegisterClassEx(&win_class)) {
        // It didn't work, so try to give a useful error:
        printf("Unexpected error trying to start the application!\n");
        fflush(stdout);
        exit(1);
    }
    // Create window with the registered class:
    RECT wr = {0, 0, demo->width, demo->height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    demo->window = CreateWindowEx(0,
                                  demo->name,           // class name
                                  demo->name,           // app name
                                  WS_OVERLAPPEDWINDOW | // window style
                                      WS_VISIBLE | WS_SYSMENU,
                                  100, 100,           // x/y coords
                                  wr.right - wr.left, // width
                                  wr.bottom - wr.top, // height
                                  NULL,               // handle to parent
                                  NULL,               // handle to menu
                                  demo->connection,   // hInstance
                                  NULL);              // no extra parameters
    if (!demo->window) {
        // It didn't work, so try to give a useful error:
        printf("Cannot create a window in which to draw!\n");
        fflush(stdout);
        exit(1);
    }
    // Window client area size must be at least 1 pixel high, to prevent crash.
    demo->minsize.x = GetSystemMetrics(SM_CXMINTRACK);
    demo->minsize.y = GetSystemMetrics(SM_CYMINTRACK)+1;
}
#elif defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
#if defined(VK_USE_PLATFORM_XLIB_KHR)
static void demo_create_xlib_window(Demo *demo) {

    demo->display = XOpenDisplay(NULL);
    long visualMask = VisualScreenMask;
    int numberOfVisuals;
    XVisualInfo vInfoTemplate={};
    vInfoTemplate.screen = DefaultScreen(demo->display);
    XVisualInfo *visualInfo = XGetVisualInfo(demo->display, visualMask,
                                             &vInfoTemplate, &numberOfVisuals);

    Colormap colormap = XCreateColormap(
                demo->display, RootWindow(demo->display, vInfoTemplate.screen),
                visualInfo->visual, AllocNone);

    XSetWindowAttributes windowAttributes={};
    windowAttributes.colormap = colormap;
    windowAttributes.background_pixel = 0xFFFFFFFF;
    windowAttributes.border_pixel = 0;
    windowAttributes.event_mask =
            KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask;

    demo->xlib_window = XCreateWindow(
                demo->display, RootWindow(demo->display, vInfoTemplate.screen), 0, 0,
                demo->width, demo->height, 0, visualInfo->depth, InputOutput,
                visualInfo->visual,
                CWBackPixel | CWBorderPixel | CWEventMask | CWColormap, &windowAttributes);

    XSelectInput(demo->display, demo->xlib_window, ExposureMask | KeyPressMask);
    XMapWindow(demo->display, demo->xlib_window);
    XFlush(demo->display);
    demo->xlib_wm_delete_window =
            XInternAtom(demo->display, "WM_DELETE_WINDOW", False);
}
static void demo_handle_xlib_event(Demo *demo, const XEvent *event) {
    switch(event->type) {
    case ClientMessage:
        if ((Atom)event->xclient.data.l[0] == demo->xlib_wm_delete_window)
            demo->quit = true;
        break;
    case KeyPress:
        switch (event->xkey.keycode) {
        case 0x9: // Escape
            demo->quit = true;
            break;
        case 0x71: // left arrow key
            demo->spin_angle += demo->spin_increment;
            break;
        case 0x72: // right arrow key
            demo->spin_angle -= demo->spin_increment;
            break;
        case 0x41:
            demo->pause = !demo->pause;
            break;
        }
        break;
    case ConfigureNotify:
        if ((demo->width != event->xconfigure.width) ||
            (demo->height != event->xconfigure.height)) {
            demo->width = event->xconfigure.width;
            demo->height = event->xconfigure.height;
            demo_resize(demo);
        }
        break;
    default:
        break;
    }

}

static void demo_run_xlib(Demo *demo) {

    while (!demo->quit) {
        XEvent event;

        if (demo->pause) {
            XNextEvent(demo->display, &event);
            demo_handle_xlib_event(demo, &event);
        } else {
            while (XPending(demo->display) > 0) {
                XNextEvent(demo->display, &event);
                demo_handle_xlib_event(demo, &event);
            }
        }

        demo_update_data_buffer(demo);
        demo_draw(demo);
        demo->curFrame++;
        if (demo->frameCount != INT32_MAX && demo->curFrame == demo->frameCount)
            demo->quit = true;
    }
}
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
static void demo_handle_xcb_event(Demo *demo,
                              const xcb_generic_event_t *event) {
    uint8_t event_code = event->response_type & 0x7f;
    switch (event_code) {
    case XCB_EXPOSE:
        // TODO: Resize window
        break;
    case XCB_CLIENT_MESSAGE:
        if ((*(xcb_client_message_event_t *)event).data.data32[0] ==
            (*demo->atom_wm_delete_window).atom) {
            demo->quit = true;
        }
        break;
    case XCB_KEY_RELEASE: {
        const xcb_key_release_event_t *key =
            (const xcb_key_release_event_t *)event;

        switch (key->detail) {
        case 0x9: // Escape
            demo->quit = true;
            break;
        case 0x71: // left arrow key
            demo->spin_angle += demo->spin_increment;
            break;
        case 0x72: // right arrow key
            demo->spin_angle -= demo->spin_increment;
            break;
        case 0x41:
            demo->pause = !demo->pause;
            break;
        }
    } break;
    case XCB_CONFIGURE_NOTIFY: {
        const xcb_configure_notify_event_t *cfg =
            (const xcb_configure_notify_event_t *)event;
        if ((demo->width != cfg->width) || (demo->height != cfg->height)) {
            demo->width = cfg->width;
            demo->height = cfg->height;
            demo_resize(demo);
        }
    } break;
    default:
        break;
    }
}

static void demo_run_xcb(Demo *demo) {
    xcb_flush(demo->connection);

    while (!demo->quit) {
        xcb_generic_event_t *event;

        if (demo->pause) {
            event = xcb_wait_for_event(demo->connection);
        } else {
            event = xcb_poll_for_event(demo->connection);
            while(event) {
                demo_handle_xcb_event(demo, event);
                free(event);
                event = xcb_poll_for_event(demo->connection);
            }
        }

        demo_update_data_buffer(demo);
        demo_draw(demo);
        demo->curFrame++;
        if (demo->frameCount != INT32_MAX && demo->curFrame == demo->frameCount)
            demo->quit = true;
    }
}

static void demo_create_xcb_window(Demo *demo) {
    uint32_t value_mask, value_list[32];

    demo->xcb_window = xcb_generate_id(demo->connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = demo->screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE |
                    XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    xcb_create_window(demo->connection, XCB_COPY_FROM_PARENT, demo->xcb_window,
                      demo->screen->root, 0, 0, demo->width, demo->height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, demo->screen->root_visual,
                      value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_cookie_t cookie =
        xcb_intern_atom(demo->connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply =
        xcb_intern_atom_reply(demo->connection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 =
        xcb_intern_atom(demo->connection, 0, 16, "WM_DELETE_WINDOW");
    demo->atom_wm_delete_window =
        xcb_intern_atom_reply(demo->connection, cookie2, 0);

    xcb_change_property(demo->connection, XCB_PROP_MODE_REPLACE, demo->xcb_window,
                        (*reply).atom, 4, 32, 1,
                        &(*demo->atom_wm_delete_window).atom);
    free(reply);

    xcb_map_window(demo->connection, demo->xcb_window);

    // Force the x/y coordinates to 100,100 results are identical in consecutive
    // runs
    const uint32_t coords[] = {100, 100};
    xcb_configure_window(demo->connection, demo->xcb_window,
                         XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
}
#endif
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
static void demo_run(Demo *demo) {
    while (!demo->quit) {
        demo_update_data_buffer(demo);
        demo_draw(demo);
        demo->curFrame++;
        if (demo->frameCount != INT32_MAX && demo->curFrame == demo->frameCount)
            demo->quit = true;
    }
}

static void handle_ping(void *data UNUSED,
                        struct wl_shell_surface *shell_surface,
                        uint32_t serial) {
    wl_shell_surface_pong(shell_surface, serial);
}

static void handle_configure(void *data UNUSED,
                             struct wl_shell_surface *shell_surface UNUSED,
                             uint32_t edges UNUSED, int32_t width UNUSED,
                             int32_t height UNUSED) {}

static void handle_popup_done(void *data UNUSED,
                              struct wl_shell_surface *shell_surface UNUSED) {}

static const struct wl_shell_surface_listener shell_surface_listener = {
    handle_ping, handle_configure, handle_popup_done};

static void demo_create_window(Demo *demo) {
    demo->window = wl_compositor_create_surface(demo->compositor);
    if (!demo->window) {
        printf("Can not create wayland_surface from compositor!\n");
        fflush(stdout);
        exit(1);
    }

    demo->shell_surface = wl_shell_get_shell_surface(demo->shell, demo->window);
    if (!demo->shell_surface) {
        printf("Can not get shell_surface from wayland_surface!\n");
        fflush(stdout);
        exit(1);
    }
    wl_shell_surface_add_listener(demo->shell_surface, &shell_surface_listener,
                                  demo);
    wl_shell_surface_set_toplevel(demo->shell_surface);
    wl_shell_surface_set_title(demo->shell_surface, APP_SHORT_NAME);
}
#endif


#if _WIN32
// Include header required for parsing the command line options.
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
                   int nCmdShow) {
    MSG msg;   // message
    bool done; // flag saying when app is complete
    int argc;
    char **argv;

    // Use the CommandLine functions to get the command line arguments.
    // Unfortunately, Microsoft outputs
    // this information as wide characters for Unicode, and we simply want the
    // Ascii version to be compatible
    // with the non-Windows side.  So, we have to convert the information to
    // Ascii character strings.
    LPWSTR *commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (NULL == commandLineArgs) {
        argc = 0;
    }

    if (argc > 0) {
        argv = (char **)malloc(sizeof(char *) * argc);
        if (argv == NULL) {
            argc = 0;
        } else {
            for (int iii = 0; iii < argc; iii++) {
                size_t wideCharLen = wcslen(commandLineArgs[iii]);
                size_t numConverted = 0;

                argv[iii] = (char *)malloc(sizeof(char) * (wideCharLen + 1));
                if (argv[iii] != NULL) {
                    wcstombs_s(&numConverted, argv[iii], wideCharLen + 1,
                               commandLineArgs[iii], wideCharLen + 1);
                }
            }
        }
    } else {
        argv = NULL;
    }

    demo_init(&demo, argc, argv);

    // Free up the items we had to allocate for the command line arguments.
    if (argc > 0 && argv != NULL) {
        for (int iii = 0; iii < argc; iii++) {
            if (argv[iii] != NULL) {
                free(argv[iii]);
            }
        }
        free(argv);
    }

    demo.connection = hInstance;
    strncpy(demo.name, "cube", APP_NAME_STR_LEN);
    demo_create_window(&demo);
    demo_init_vk_swapchain(&demo);

    demo_prepare(&demo);

    done = false; // initialize loop condition variable

    // main message loop
    while (!done) {
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        if (msg.message == WM_QUIT) // check for a quit message
        {
            done = true; // if found, quit app
        } else {
            /* Translate and dispatch to event queue*/
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        RedrawWindow(demo.window, NULL, NULL, RDW_INTERNALPAINT);
    }

    demo_cleanup(&demo);

    return (int)msg.wParam;
}
#elif __linux__
int main(int argc, char **argv)
{
    Demo demo;

    demo_init(&demo, argc, argv);
#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
    if (demo.use_xlib)
        demo_create_xlib_window(&demo);
    else
        demo_create_xcb_window(&demo);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    demo_create_xcb_window(&demo);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    demo_create_xlib_window(&demo);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    demo_create_window(&demo);
#endif

    demo_init_vk_swapchain(&demo);

    demo_prepare(&demo);

#if defined(VK_USE_PLATFORM_XLIB_KHR) && defined(VK_USE_PLATFORM_XCB_KHR)
    if (demo.use_xlib)
        demo_run_xlib(&demo);
    else
        demo_run_xcb(&demo);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    demo_run_xcb(&demo);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    demo_run_xlib(&demo);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
    demo_run(&demo);
#endif

    demo_cleanup(&demo);

    return validation_error;
}
#else
#error "Platform not supported"
#endif
