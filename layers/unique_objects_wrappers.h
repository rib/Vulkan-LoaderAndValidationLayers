#ifndef __unique_objects_wrappers_h_
#define __unique_objects_wrappers_h_ 1

namespace unique_objects {

/*
** Copyright (c) 2015-2016 The Khronos Group Inc.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

/*
** This header is generated from the Khronos Vulkan XML API Registry.
**
*/


// declare only
    VKAPI_ATTR VkResult VKAPI_CALL CreateInstance(
        const VkInstanceCreateInfo*                 pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkInstance*                                 pInstance);

// declare only
VKAPI_ATTR void VKAPI_CALL DestroyInstance(
    VkInstance                                  instance,
    const VkAllocationCallbacks*                pAllocator);


// declare only
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName);

// declare only
VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(
    VkDevice                                    device,
    const char*                                 pName);

// declare only
VKAPI_ATTR VkResult VKAPI_CALL CreateDevice(
    VkPhysicalDevice                            physicalDevice,
    const VkDeviceCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDevice*                                   pDevice);

// declare only
VKAPI_ATTR void VKAPI_CALL DestroyDevice(
    VkDevice                                    device,
    const VkAllocationCallbacks*                pAllocator);

// declare only
VKAPI_ATTR VkResult VKAPI_CALL AllocateCommandBuffers(
    VkDevice                                    device,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers);

// declare only
VKAPI_ATTR void VKAPI_CALL FreeCommandBuffers(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers);

VKAPI_ATTR VkResult VKAPI_CALL CreateGraphicsPipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

VKAPI_ATTR VkResult VKAPI_CALL CreateComputePipelines(VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines);

VKAPI_ATTR VkResult VKAPI_CALL AllocateMemory(VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo, const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory);

// declare only
VKAPI_ATTR VkResult VKAPI_CALL CreateDebugReportCallbackEXT(
    VkInstance                                  instance,
    const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDebugReportCallbackEXT*                   pCallback);

// declare only
VKAPI_ATTR void VKAPI_CALL DestroyDebugReportCallbackEXT(
    VkInstance                                  instance,
    VkDebugReportCallbackEXT                    callback,
    const VkAllocationCallbacks*                pAllocator);

VKAPI_ATTR void VKAPI_CALL DebugReportMessageEXT(
    VkInstance                                  instance,
    VkDebugReportFlagsEXT                       flags,
    VkDebugReportObjectTypeEXT                  objectType,
    uint64_t                                    object,
    size_t                                      location,
    int32_t                                     messageCode,
    const char*                                 pLayerPrefix,
    const char*                                 pMessage)
{
    dispatch_key key = get_dispatch_key(instance);
    layer_data *my_data = get_my_data_ptr(key, layer_data_map);
    VkLayerInstanceDispatchTable *pTable = my_data->instance_dispatch_table;
    pTable->DebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}


VKAPI_ATTR VkResult VKAPI_CALL QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(queue), layer_data_map);
// STRUCT USES:['fence', 'pSubmits[submitCount]']
//LOCAL DECLS:['pSubmits']
    safe_VkSubmitInfo* local_pSubmits = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    fence = (VkFence)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(fence)];
    if (pSubmits) {
        local_pSubmits = new safe_VkSubmitInfo[submitCount];
        for (uint32_t idx0=0; idx0<submitCount; ++idx0) {
            local_pSubmits[idx0].initialize(&pSubmits[idx0]);
            if (local_pSubmits[idx0].pSignalSemaphores) {
                for (uint32_t idx1=0; idx1<pSubmits[idx0].signalSemaphoreCount; ++idx1) {
                    local_pSubmits[idx0].pSignalSemaphores[idx1] = (VkSemaphore)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pSubmits[idx0].pSignalSemaphores[idx1])];
                }
            }
            if (local_pSubmits[idx0].pWaitSemaphores) {
                for (uint32_t idx2=0; idx2<pSubmits[idx0].waitSemaphoreCount; ++idx2) {
                    local_pSubmits[idx0].pWaitSemaphores[idx2] = (VkSemaphore)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pSubmits[idx0].pWaitSemaphores[idx2])];
                }
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->QueueSubmit(queue, submitCount, (const VkSubmitInfo*)local_pSubmits, fence);
    if (local_pSubmits)
        delete[] local_pSubmits;
    return result;
}


VKAPI_ATTR void VKAPI_CALL FreeMemory(VkDevice device, VkDeviceMemory memory, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['memory']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_memory = reinterpret_cast<uint64_t &>(memory);
    memory = (VkDeviceMemory)my_map_data->unique_id_mapping[local_memory];
    my_map_data->unique_id_mapping.erase(local_memory);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->FreeMemory(device, memory, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL MapMemory(VkDevice device, VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['memory']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(memory)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->MapMemory(device, memory, offset, size, flags, ppData);
    return result;
}


VKAPI_ATTR void VKAPI_CALL UnmapMemory(VkDevice device, VkDeviceMemory memory)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['memory']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(memory)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->UnmapMemory(device, memory);
}


VKAPI_ATTR VkResult VKAPI_CALL FlushMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pMemoryRanges[memoryRangeCount]']
    //LOCAL DECLS:['pMemoryRanges']
    safe_VkMappedMemoryRange* local_pMemoryRanges = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pMemoryRanges) {
            local_pMemoryRanges = new safe_VkMappedMemoryRange[memoryRangeCount];
            for (uint32_t idx0 = 0; idx0<memoryRangeCount; ++idx0) {
                local_pMemoryRanges[idx0].initialize(&pMemoryRanges[idx0]);
                if (pMemoryRanges[idx0].memory) {
                    local_pMemoryRanges[idx0].memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pMemoryRanges[idx0].memory)];
                }
            }
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->FlushMappedMemoryRanges(device, memoryRangeCount, (const VkMappedMemoryRange*)local_pMemoryRanges);
    if (local_pMemoryRanges)
        delete[] local_pMemoryRanges;
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL InvalidateMappedMemoryRanges(VkDevice device, uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pMemoryRanges[memoryRangeCount]']
    //LOCAL DECLS:['pMemoryRanges']
    safe_VkMappedMemoryRange* local_pMemoryRanges = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pMemoryRanges) {
            local_pMemoryRanges = new safe_VkMappedMemoryRange[memoryRangeCount];
            for (uint32_t idx0 = 0; idx0<memoryRangeCount; ++idx0) {
                local_pMemoryRanges[idx0].initialize(&pMemoryRanges[idx0]);
                if (pMemoryRanges[idx0].memory) {
                    local_pMemoryRanges[idx0].memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pMemoryRanges[idx0].memory)];
                }
            }
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->InvalidateMappedMemoryRanges(device, memoryRangeCount, (const VkMappedMemoryRange*)local_pMemoryRanges);
    if (local_pMemoryRanges)
        delete[] local_pMemoryRanges;
    return result;
}


VKAPI_ATTR void VKAPI_CALL GetDeviceMemoryCommitment(VkDevice device, VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['memory']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(memory)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}


VKAPI_ATTR VkResult VKAPI_CALL BindBufferMemory(VkDevice device, VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['buffer', 'memory']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(memory)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->BindBufferMemory(device, buffer, memory, memoryOffset);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL BindImageMemory(VkDevice device, VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['image', 'memory']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(image)];
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(memory)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->BindImageMemory(device, image, memory, memoryOffset);
    return result;
}


VKAPI_ATTR void VKAPI_CALL GetBufferMemoryRequirements(VkDevice device, VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['buffer']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}


VKAPI_ATTR void VKAPI_CALL GetImageMemoryRequirements(VkDevice device, VkImage image, VkMemoryRequirements* pMemoryRequirements)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['image']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(image)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->GetImageMemoryRequirements(device, image, pMemoryRequirements);
}


VKAPI_ATTR void VKAPI_CALL GetImageSparseMemoryRequirements(VkDevice device, VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['image']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(image)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}


VKAPI_ATTR VkResult VKAPI_CALL QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(queue), layer_data_map);
    // STRUCT USES:['fence', 'pBindInfo[bindInfoCount]']
    //LOCAL DECLS:['pBindInfo']
    safe_VkBindSparseInfo* local_pBindInfo = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        fence = (VkFence)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(fence)];
        if (pBindInfo) {
            local_pBindInfo = new safe_VkBindSparseInfo[bindInfoCount];
            for (uint32_t idx0 = 0; idx0<bindInfoCount; ++idx0) {
                local_pBindInfo[idx0].initialize(&pBindInfo[idx0]);
                if (local_pBindInfo[idx0].pBufferBinds) {
                    for (uint32_t idx1 = 0; idx1<pBindInfo[idx0].bufferBindCount; ++idx1) {
                        if (pBindInfo[idx0].pBufferBinds[idx1].buffer) {
                            local_pBindInfo[idx0].pBufferBinds[idx1].buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pBufferBinds[idx1].buffer)];
                        }
                        if (local_pBindInfo[idx0].pBufferBinds[idx1].pBinds) {
                            for (uint32_t idx2 = 0; idx2<pBindInfo[idx0].pBufferBinds[idx1].bindCount; ++idx2) {
                                if (pBindInfo[idx0].pBufferBinds[idx1].pBinds[idx2].memory) {
                                    local_pBindInfo[idx0].pBufferBinds[idx1].pBinds[idx2].memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pBufferBinds[idx1].pBinds[idx2].memory)];
                                }
                            }
                        }
                    }
                }
                if (local_pBindInfo[idx0].pImageBinds) {
                    for (uint32_t idx2 = 0; idx2<pBindInfo[idx0].imageBindCount; ++idx2) {
                        if (pBindInfo[idx0].pImageBinds[idx2].image) {
                            local_pBindInfo[idx0].pImageBinds[idx2].image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pImageBinds[idx2].image)];
                        }
                        if (local_pBindInfo[idx0].pImageBinds[idx2].pBinds) {
                            for (uint32_t idx3 = 0; idx3<pBindInfo[idx0].pImageBinds[idx2].bindCount; ++idx3) {
                                if (pBindInfo[idx0].pImageBinds[idx2].pBinds[idx3].memory) {
                                    local_pBindInfo[idx0].pImageBinds[idx2].pBinds[idx3].memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pImageBinds[idx2].pBinds[idx3].memory)];
                                }
                            }
                        }
                    }
                }
                if (local_pBindInfo[idx0].pImageOpaqueBinds) {
                    for (uint32_t idx3 = 0; idx3<pBindInfo[idx0].imageOpaqueBindCount; ++idx3) {
                        if (pBindInfo[idx0].pImageOpaqueBinds[idx3].image) {
                            local_pBindInfo[idx0].pImageOpaqueBinds[idx3].image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pImageOpaqueBinds[idx3].image)];
                        }
                        if (local_pBindInfo[idx0].pImageOpaqueBinds[idx3].pBinds) {
                            for (uint32_t idx4 = 0; idx4<pBindInfo[idx0].pImageOpaqueBinds[idx3].bindCount; ++idx4) {
                                if (pBindInfo[idx0].pImageOpaqueBinds[idx3].pBinds[idx4].memory) {
                                    local_pBindInfo[idx0].pImageOpaqueBinds[idx3].pBinds[idx4].memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pImageOpaqueBinds[idx3].pBinds[idx4].memory)];
                                }
                            }
                        }
                    }
                }
                if (local_pBindInfo[idx0].pSignalSemaphores) {
                    for (uint32_t idx4 = 0; idx4<pBindInfo[idx0].signalSemaphoreCount; ++idx4) {
                        local_pBindInfo[idx0].pSignalSemaphores[idx4] = (VkSemaphore)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pSignalSemaphores[idx4])];
                    }
                }
                if (local_pBindInfo[idx0].pWaitSemaphores) {
                    for (uint32_t idx5 = 0; idx5<pBindInfo[idx0].waitSemaphoreCount; ++idx5) {
                        local_pBindInfo[idx0].pWaitSemaphores[idx5] = (VkSemaphore)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBindInfo[idx0].pWaitSemaphores[idx5])];
                    }
                }
            }
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->QueueBindSparse(queue, bindInfoCount, (const VkBindSparseInfo*)local_pBindInfo, fence);
    if (local_pBindInfo)
        delete[] local_pBindInfo;
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateFence(VkDevice device, const VkFenceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFence* pFence)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateFence(device, pCreateInfo, pAllocator, pFence);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pFence);
        *pFence = reinterpret_cast<VkFence&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyFence(VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['fence']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_fence = reinterpret_cast<uint64_t &>(fence);
    fence = (VkFence)my_map_data->unique_id_mapping[local_fence];
    my_map_data->unique_id_mapping.erase(local_fence);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyFence(device, fence, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL ResetFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pFences[fenceCount]']
    //LOCAL DECLS:['pFences']
    VkFence* local_pFences = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pFences) {
            local_pFences = new VkFence[fenceCount];
            for (uint32_t idx0 = 0; idx0<fenceCount; ++idx0) {
                local_pFences[idx0] = (VkFence)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pFences[idx0])];
            }
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->ResetFences(device, fenceCount, (const VkFence*)local_pFences);
    if (local_pFences)
        delete[] local_pFences;
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetFenceStatus(VkDevice device, VkFence fence)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['fence']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        fence = (VkFence)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(fence)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->GetFenceStatus(device, fence);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL WaitForFences(VkDevice device, uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pFences[fenceCount]']
    //LOCAL DECLS:['pFences']
    VkFence* local_pFences = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pFences) {
            local_pFences = new VkFence[fenceCount];
            for (uint32_t idx0 = 0; idx0<fenceCount; ++idx0) {
                local_pFences[idx0] = (VkFence)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pFences[idx0])];
            }
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->WaitForFences(device, fenceCount, (const VkFence*)local_pFences, waitAll, timeout);
    if (local_pFences)
        delete[] local_pFences;
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateSemaphore(VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pSemaphore);
        *pSemaphore = reinterpret_cast<VkSemaphore&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroySemaphore(VkDevice device, VkSemaphore semaphore, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['semaphore']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_semaphore = reinterpret_cast<uint64_t &>(semaphore);
    semaphore = (VkSemaphore)my_map_data->unique_id_mapping[local_semaphore];
    my_map_data->unique_id_mapping.erase(local_semaphore);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroySemaphore(device, semaphore, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateEvent(device, pCreateInfo, pAllocator, pEvent);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pEvent);
        *pEvent = reinterpret_cast<VkEvent&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['event']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_event = reinterpret_cast<uint64_t &>(event);
    event = (VkEvent)my_map_data->unique_id_mapping[local_event];
    my_map_data->unique_id_mapping.erase(local_event);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyEvent(device, event, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL GetEventStatus(VkDevice device, VkEvent event)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['event']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        event = (VkEvent)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(event)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->GetEventStatus(device, event);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL SetEvent(VkDevice device, VkEvent event)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['event']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        event = (VkEvent)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(event)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->SetEvent(device, event);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL ResetEvent(VkDevice device, VkEvent event)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['event']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        event = (VkEvent)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(event)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->ResetEvent(device, event);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateQueryPool(VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pQueryPool);
        *pQueryPool = reinterpret_cast<VkQueryPool&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyQueryPool(VkDevice device, VkQueryPool queryPool, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['queryPool']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_queryPool = reinterpret_cast<uint64_t &>(queryPool);
    queryPool = (VkQueryPool)my_map_data->unique_id_mapping[local_queryPool];
    my_map_data->unique_id_mapping.erase(local_queryPool);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyQueryPool(device, queryPool, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL GetQueryPoolResults(VkDevice device, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['queryPool']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        queryPool = (VkQueryPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(queryPool)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateBuffer(VkDevice device, const VkBufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pBuffer);
        *pBuffer = reinterpret_cast<VkBuffer&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyBuffer(VkDevice device, VkBuffer buffer, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['buffer']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_buffer = reinterpret_cast<uint64_t &>(buffer);
    buffer = (VkBuffer)my_map_data->unique_id_mapping[local_buffer];
    my_map_data->unique_id_mapping.erase(local_buffer);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyBuffer(device, buffer, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateBufferView(VkDevice device, const VkBufferViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkBufferView* pView)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pCreateInfo']
    //LOCAL DECLS:['pCreateInfo']
    safe_VkBufferViewCreateInfo* local_pCreateInfo = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pCreateInfo) {
            local_pCreateInfo = new safe_VkBufferViewCreateInfo(pCreateInfo);
            // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
            local_pCreateInfo->buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->buffer)];
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateBufferView(device, (const VkBufferViewCreateInfo*)local_pCreateInfo, pAllocator, pView);
    if (local_pCreateInfo)
        delete local_pCreateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pView);
        *pView = reinterpret_cast<VkBufferView&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyBufferView(VkDevice device, VkBufferView bufferView, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['bufferView']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_bufferView = reinterpret_cast<uint64_t &>(bufferView);
    bufferView = (VkBufferView)my_map_data->unique_id_mapping[local_bufferView];
    my_map_data->unique_id_mapping.erase(local_bufferView);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyBufferView(device, bufferView, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateImage(VkDevice device, const VkImageCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImage* pImage)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateImage(device, pCreateInfo, pAllocator, pImage);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pImage);
        *pImage = reinterpret_cast<VkImage&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyImage(VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['image']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_image = reinterpret_cast<uint64_t &>(image);
    image = (VkImage)my_map_data->unique_id_mapping[local_image];
    my_map_data->unique_id_mapping.erase(local_image);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyImage(device, image, pAllocator);
}


VKAPI_ATTR void VKAPI_CALL GetImageSubresourceLayout(VkDevice device, VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['image']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(image)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->GetImageSubresourceLayout(device, image, pSubresource, pLayout);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateImageView(VkDevice device, const VkImageViewCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkImageView* pView)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pCreateInfo']
    //LOCAL DECLS:['pCreateInfo']
    safe_VkImageViewCreateInfo* local_pCreateInfo = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pCreateInfo) {
            local_pCreateInfo = new safe_VkImageViewCreateInfo(pCreateInfo);
            // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
            local_pCreateInfo->image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->image)];
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateImageView(device, (const VkImageViewCreateInfo*)local_pCreateInfo, pAllocator, pView);

    if (local_pCreateInfo)
        delete local_pCreateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pView);
        *pView = reinterpret_cast<VkImageView&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyImageView(VkDevice device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['imageView']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_imageView = reinterpret_cast<uint64_t &>(imageView);
    imageView = (VkImageView)my_map_data->unique_id_mapping[local_imageView];
    my_map_data->unique_id_mapping.erase(local_imageView);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyImageView(device, imageView, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateShaderModule(VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pShaderModule);
        *pShaderModule = reinterpret_cast<VkShaderModule&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyShaderModule(VkDevice device, VkShaderModule shaderModule, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['shaderModule']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_shaderModule = reinterpret_cast<uint64_t &>(shaderModule);
    shaderModule = (VkShaderModule)my_map_data->unique_id_mapping[local_shaderModule];
    my_map_data->unique_id_mapping.erase(local_shaderModule);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyShaderModule(device, shaderModule, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreatePipelineCache(VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pPipelineCache);
        *pPipelineCache = reinterpret_cast<VkPipelineCache&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyPipelineCache(VkDevice device, VkPipelineCache pipelineCache, const VkAllocationCallbacks* pAllocator)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pipelineCache']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_pipelineCache = reinterpret_cast<uint64_t &>(pipelineCache);
    pipelineCache = (VkPipelineCache)my_map_data->unique_id_mapping[local_pipelineCache];
    my_map_data->unique_id_mapping.erase(local_pipelineCache);
    lock.unlock();
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyPipelineCache(device, pipelineCache, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL GetPipelineCacheData(VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize, void* pData)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['pipelineCache']
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        pipelineCache = (VkPipelineCache)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(pipelineCache)];
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL MergePipelineCaches(VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
    // STRUCT USES:['dstCache', 'pSrcCaches[srcCacheCount]']
    //LOCAL DECLS:['pSrcCaches']
    VkPipelineCache* local_pSrcCaches = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        dstCache = (VkPipelineCache)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstCache)];
        if (pSrcCaches) {
            local_pSrcCaches = new VkPipelineCache[srcCacheCount];
            for (uint32_t idx0 = 0; idx0<srcCacheCount; ++idx0) {
                local_pSrcCaches[idx0] = (VkPipelineCache)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pSrcCaches[idx0])];
            }
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->MergePipelineCaches(device, dstCache, srcCacheCount, (const VkPipelineCache*)local_pSrcCaches);
    if (local_pSrcCaches)
        delete[] local_pSrcCaches;
    return result;
}




VKAPI_ATTR void VKAPI_CALL DestroyPipeline(VkDevice device, VkPipeline pipeline, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pipeline']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_pipeline = reinterpret_cast<uint64_t &>(pipeline);
    pipeline = (VkPipeline)my_map_data->unique_id_mapping[local_pipeline];
    my_map_data->unique_id_mapping.erase(local_pipeline);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyPipeline(device, pipeline, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreatePipelineLayout(VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pCreateInfo']
//LOCAL DECLS:['pCreateInfo']
    safe_VkPipelineLayoutCreateInfo* local_pCreateInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pCreateInfo) {
        local_pCreateInfo = new safe_VkPipelineLayoutCreateInfo(pCreateInfo);
        if (local_pCreateInfo->pSetLayouts) {
            for (uint32_t idx0=0; idx0<pCreateInfo->setLayoutCount; ++idx0) {
                local_pCreateInfo->pSetLayouts[idx0] = (VkDescriptorSetLayout)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->pSetLayouts[idx0])];
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreatePipelineLayout(device, (const VkPipelineLayoutCreateInfo*)local_pCreateInfo, pAllocator, pPipelineLayout);
    if (local_pCreateInfo)
        delete local_pCreateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pPipelineLayout);
        *pPipelineLayout = reinterpret_cast<VkPipelineLayout&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pipelineLayout']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_pipelineLayout = reinterpret_cast<uint64_t &>(pipelineLayout);
    pipelineLayout = (VkPipelineLayout)my_map_data->unique_id_mapping[local_pipelineLayout];
    my_map_data->unique_id_mapping.erase(local_pipelineLayout);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyPipelineLayout(device, pipelineLayout, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateSampler(VkDevice device, const VkSamplerCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSampler* pSampler)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateSampler(device, pCreateInfo, pAllocator, pSampler);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pSampler);
        *pSampler = reinterpret_cast<VkSampler&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroySampler(VkDevice device, VkSampler sampler, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['sampler']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_sampler = reinterpret_cast<uint64_t &>(sampler);
    sampler = (VkSampler)my_map_data->unique_id_mapping[local_sampler];
    my_map_data->unique_id_mapping.erase(local_sampler);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroySampler(device, sampler, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorSetLayout(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorSetLayout* pSetLayout)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pCreateInfo']
//LOCAL DECLS:['pCreateInfo']
    safe_VkDescriptorSetLayoutCreateInfo* local_pCreateInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pCreateInfo) {
        local_pCreateInfo = new safe_VkDescriptorSetLayoutCreateInfo(pCreateInfo);
        if (local_pCreateInfo->pBindings) {
            for (uint32_t idx0=0; idx0<pCreateInfo->bindingCount; ++idx0) {
                if (local_pCreateInfo->pBindings[idx0].pImmutableSamplers) {
                    for (uint32_t idx1=0; idx1<pCreateInfo->pBindings[idx0].descriptorCount; ++idx1) {
                        local_pCreateInfo->pBindings[idx0].pImmutableSamplers[idx1] = (VkSampler)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->pBindings[idx0].pImmutableSamplers[idx1])];
                    }
                }
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateDescriptorSetLayout(device, (const VkDescriptorSetLayoutCreateInfo*)local_pCreateInfo, pAllocator, pSetLayout);
    if (local_pCreateInfo)
        delete local_pCreateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pSetLayout);
        *pSetLayout = reinterpret_cast<VkDescriptorSetLayout&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['descriptorSetLayout']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_descriptorSetLayout = reinterpret_cast<uint64_t &>(descriptorSetLayout);
    descriptorSetLayout = (VkDescriptorSetLayout)my_map_data->unique_id_mapping[local_descriptorSetLayout];
    my_map_data->unique_id_mapping.erase(local_descriptorSetLayout);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorPool(VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pDescriptorPool);
        *pDescriptorPool = reinterpret_cast<VkDescriptorPool&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['descriptorPool']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_descriptorPool = reinterpret_cast<uint64_t &>(descriptorPool);
    descriptorPool = (VkDescriptorPool)my_map_data->unique_id_mapping[local_descriptorPool];
    my_map_data->unique_id_mapping.erase(local_descriptorPool);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyDescriptorPool(device, descriptorPool, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL ResetDescriptorPool(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['descriptorPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    descriptorPool = (VkDescriptorPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(descriptorPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->ResetDescriptorPool(device, descriptorPool, flags);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL AllocateDescriptorSets(VkDevice device, const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pAllocateInfo']
//LOCAL DECLS:['pAllocateInfo']
    safe_VkDescriptorSetAllocateInfo* local_pAllocateInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pAllocateInfo) {
        local_pAllocateInfo = new safe_VkDescriptorSetAllocateInfo(pAllocateInfo);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        local_pAllocateInfo->descriptorPool = (VkDescriptorPool)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pAllocateInfo->descriptorPool)];
        if (local_pAllocateInfo->pSetLayouts) {
            for (uint32_t idx0=0; idx0<pAllocateInfo->descriptorSetCount; ++idx0) {
                local_pAllocateInfo->pSetLayouts[idx0] = (VkDescriptorSetLayout)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pAllocateInfo->pSetLayouts[idx0])];
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->AllocateDescriptorSets(device, (const VkDescriptorSetAllocateInfo*)local_pAllocateInfo, pDescriptorSets);
    if (local_pAllocateInfo)
        delete local_pAllocateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #951
        for (uint32_t i=0; i<pAllocateInfo->descriptorSetCount; ++i) {
            uint64_t unique_id = global_unique_id++;
            my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(pDescriptorSets[i]);
            pDescriptorSets[i] = reinterpret_cast<VkDescriptorSet&>(unique_id);
        }
    }
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL FreeDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['descriptorPool', 'pDescriptorSets[descriptorSetCount]']
//LOCAL DECLS:['pDescriptorSets']
    VkDescriptorSet* local_pDescriptorSets = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    descriptorPool = (VkDescriptorPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(descriptorPool)];
    if (pDescriptorSets) {
        local_pDescriptorSets = new VkDescriptorSet[descriptorSetCount];
        for (uint32_t idx0=0; idx0<descriptorSetCount; ++idx0) {
            local_pDescriptorSets[idx0] = (VkDescriptorSet)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorSets[idx0])];
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->FreeDescriptorSets(device, descriptorPool, descriptorSetCount, (const VkDescriptorSet*)local_pDescriptorSets);
    if (local_pDescriptorSets)
        delete[] local_pDescriptorSets;
    return result;
}


VKAPI_ATTR void VKAPI_CALL UpdateDescriptorSets(VkDevice device, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pDescriptorCopies[descriptorCopyCount]', 'pDescriptorWrites[descriptorWriteCount]']
//LOCAL DECLS:['pDescriptorCopies', 'pDescriptorWrites']
    safe_VkCopyDescriptorSet* local_pDescriptorCopies = NULL;
    safe_VkWriteDescriptorSet* local_pDescriptorWrites = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pDescriptorCopies) {
        local_pDescriptorCopies = new safe_VkCopyDescriptorSet[descriptorCopyCount];
        for (uint32_t idx0=0; idx0<descriptorCopyCount; ++idx0) {
            local_pDescriptorCopies[idx0].initialize(&pDescriptorCopies[idx0]);
            if (pDescriptorCopies[idx0].dstSet) {
                local_pDescriptorCopies[idx0].dstSet = (VkDescriptorSet)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorCopies[idx0].dstSet)];
            }
            if (pDescriptorCopies[idx0].srcSet) {
                local_pDescriptorCopies[idx0].srcSet = (VkDescriptorSet)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorCopies[idx0].srcSet)];
            }
        }
    }
    if (pDescriptorWrites) {
        local_pDescriptorWrites = new safe_VkWriteDescriptorSet[descriptorWriteCount];
        for (uint32_t idx1=0; idx1<descriptorWriteCount; ++idx1) {
            local_pDescriptorWrites[idx1].initialize(&pDescriptorWrites[idx1]);
            if (pDescriptorWrites[idx1].dstSet) {
                local_pDescriptorWrites[idx1].dstSet = (VkDescriptorSet)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorWrites[idx1].dstSet)];
            }
            if (local_pDescriptorWrites[idx1].pBufferInfo) {
                for (uint32_t idx2=0; idx2<pDescriptorWrites[idx1].descriptorCount; ++idx2) {
                    if (pDescriptorWrites[idx1].pBufferInfo[idx2].buffer) {
                        local_pDescriptorWrites[idx1].pBufferInfo[idx2].buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorWrites[idx1].pBufferInfo[idx2].buffer)];
                    }
                }
            }
            if (local_pDescriptorWrites[idx1].pImageInfo) {
                for (uint32_t idx3=0; idx3<pDescriptorWrites[idx1].descriptorCount; ++idx3) {
                    if (pDescriptorWrites[idx1].pImageInfo[idx3].imageView) {
                        local_pDescriptorWrites[idx1].pImageInfo[idx3].imageView = (VkImageView)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorWrites[idx1].pImageInfo[idx3].imageView)];
                    }
                    if (pDescriptorWrites[idx1].pImageInfo[idx3].sampler) {
                        local_pDescriptorWrites[idx1].pImageInfo[idx3].sampler = (VkSampler)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorWrites[idx1].pImageInfo[idx3].sampler)];
                    }
                }
            }
            if (local_pDescriptorWrites[idx1].pTexelBufferView) {
                for (uint32_t idx4=0; idx4<pDescriptorWrites[idx1].descriptorCount; ++idx4) {
                    local_pDescriptorWrites[idx1].pTexelBufferView[idx4] = (VkBufferView)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorWrites[idx1].pTexelBufferView[idx4])];
                }
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->UpdateDescriptorSets(device, descriptorWriteCount, (const VkWriteDescriptorSet*)local_pDescriptorWrites, descriptorCopyCount, (const VkCopyDescriptorSet*)local_pDescriptorCopies);
    if (local_pDescriptorCopies)
        delete[] local_pDescriptorCopies;
    if (local_pDescriptorWrites)
        delete[] local_pDescriptorWrites;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateFramebuffer(VkDevice device, const VkFramebufferCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pCreateInfo']
//LOCAL DECLS:['pCreateInfo']
    safe_VkFramebufferCreateInfo* local_pCreateInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pCreateInfo) {
        local_pCreateInfo = new safe_VkFramebufferCreateInfo(pCreateInfo);
        if (local_pCreateInfo->pAttachments) {
            for (uint32_t idx0=0; idx0<pCreateInfo->attachmentCount; ++idx0) {
                local_pCreateInfo->pAttachments[idx0] = (VkImageView)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->pAttachments[idx0])];
            }
        }
        if (pCreateInfo->renderPass) {
            local_pCreateInfo->renderPass = (VkRenderPass)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->renderPass)];
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateFramebuffer(device, (const VkFramebufferCreateInfo*)local_pCreateInfo, pAllocator, pFramebuffer);
    if (local_pCreateInfo)
        delete local_pCreateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pFramebuffer);
        *pFramebuffer = reinterpret_cast<VkFramebuffer&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyFramebuffer(VkDevice device, VkFramebuffer framebuffer, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['framebuffer']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_framebuffer = reinterpret_cast<uint64_t &>(framebuffer);
    framebuffer = (VkFramebuffer)my_map_data->unique_id_mapping[local_framebuffer];
    my_map_data->unique_id_mapping.erase(local_framebuffer);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyFramebuffer(device, framebuffer, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateRenderPass(VkDevice device, const VkRenderPassCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pRenderPass);
        *pRenderPass = reinterpret_cast<VkRenderPass&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyRenderPass(VkDevice device, VkRenderPass renderPass, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['renderPass']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_renderPass = reinterpret_cast<uint64_t &>(renderPass);
    renderPass = (VkRenderPass)my_map_data->unique_id_mapping[local_renderPass];
    my_map_data->unique_id_mapping.erase(local_renderPass);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyRenderPass(device, renderPass, pAllocator);
}


VKAPI_ATTR void VKAPI_CALL GetRenderAreaGranularity(VkDevice device, VkRenderPass renderPass, VkExtent2D* pGranularity)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['renderPass']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    renderPass = (VkRenderPass)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(renderPass)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->GetRenderAreaGranularity(device, renderPass, pGranularity);
}


VKAPI_ATTR VkResult VKAPI_CALL CreateCommandPool(VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pCommandPool);
        *pCommandPool = reinterpret_cast<VkCommandPool&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL DestroyCommandPool(VkDevice device, VkCommandPool commandPool, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['commandPool']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_commandPool = reinterpret_cast<uint64_t &>(commandPool);
    commandPool = (VkCommandPool)my_map_data->unique_id_mapping[local_commandPool];
    my_map_data->unique_id_mapping.erase(local_commandPool);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroyCommandPool(device, commandPool, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL ResetCommandPool(VkDevice device, VkCommandPool commandPool, VkCommandPoolResetFlags flags)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['commandPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    commandPool = (VkCommandPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(commandPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->ResetCommandPool(device, commandPool, flags);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL AllocateCommandBuffers(VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['pAllocateInfo']
//LOCAL DECLS:['pAllocateInfo']
    safe_VkCommandBufferAllocateInfo* local_pAllocateInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pAllocateInfo) {
        local_pAllocateInfo = new safe_VkCommandBufferAllocateInfo(pAllocateInfo);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        local_pAllocateInfo->commandPool = (VkCommandPool)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pAllocateInfo->commandPool)];
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->AllocateCommandBuffers(device, (const VkCommandBufferAllocateInfo*)local_pAllocateInfo, pCommandBuffers);
    if (local_pAllocateInfo)
        delete local_pAllocateInfo;
    return result;
}


VKAPI_ATTR void VKAPI_CALL FreeCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['commandPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    commandPool = (VkCommandPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(commandPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}


VKAPI_ATTR VkResult VKAPI_CALL BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['pBeginInfo']
//LOCAL DECLS:['pBeginInfo']
    safe_VkCommandBufferBeginInfo* local_pBeginInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pBeginInfo) {
        local_pBeginInfo = new safe_VkCommandBufferBeginInfo(pBeginInfo);
        if (local_pBeginInfo->pInheritanceInfo) {
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
            local_pBeginInfo->pInheritanceInfo->framebuffer = (VkFramebuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBeginInfo->pInheritanceInfo->framebuffer)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
            local_pBeginInfo->pInheritanceInfo->renderPass = (VkRenderPass)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBeginInfo->pInheritanceInfo->renderPass)];
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->BeginCommandBuffer(commandBuffer, (const VkCommandBufferBeginInfo*)local_pBeginInfo);
    if (local_pBeginInfo)
        delete local_pBeginInfo;
    return result;
}


VKAPI_ATTR void VKAPI_CALL CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['pipeline']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    pipeline = (VkPipeline)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(pipeline)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}


VKAPI_ATTR void VKAPI_CALL CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['layout', 'pDescriptorSets[descriptorSetCount]']
//LOCAL DECLS:['pDescriptorSets']
    VkDescriptorSet* local_pDescriptorSets = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    layout = (VkPipelineLayout)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(layout)];
    if (pDescriptorSets) {
        local_pDescriptorSets = new VkDescriptorSet[descriptorSetCount];
        for (uint32_t idx0=0; idx0<descriptorSetCount; ++idx0) {
            local_pDescriptorSets[idx0] = (VkDescriptorSet)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pDescriptorSets[idx0])];
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, (const VkDescriptorSet*)local_pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    if (local_pDescriptorSets)
        delete[] local_pDescriptorSets;
}


VKAPI_ATTR void VKAPI_CALL CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['buffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}


VKAPI_ATTR void VKAPI_CALL CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['pBuffers[bindingCount]']
//LOCAL DECLS:['pBuffers']
    VkBuffer* local_pBuffers = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pBuffers) {
        local_pBuffers = new VkBuffer[bindingCount];
        for (uint32_t idx0=0; idx0<bindingCount; ++idx0) {
            local_pBuffers[idx0] = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBuffers[idx0])];
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, (const VkBuffer*)local_pBuffers, pOffsets);
    if (local_pBuffers)
        delete[] local_pBuffers;
}


VKAPI_ATTR void VKAPI_CALL CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['buffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}


VKAPI_ATTR void VKAPI_CALL CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['buffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}


VKAPI_ATTR void VKAPI_CALL CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['buffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdDispatchIndirect(commandBuffer, buffer, offset);
}


VKAPI_ATTR void VKAPI_CALL CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstBuffer', 'srcBuffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstBuffer)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    srcBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(srcBuffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}


VKAPI_ATTR void VKAPI_CALL CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstImage', 'srcImage']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstImage)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    srcImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(srcImage)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}


VKAPI_ATTR void VKAPI_CALL CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstImage', 'srcImage']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstImage)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    srcImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(srcImage)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}


VKAPI_ATTR void VKAPI_CALL CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstImage', 'srcBuffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstImage)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    srcBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(srcBuffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}


VKAPI_ATTR void VKAPI_CALL CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstBuffer', 'srcImage']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstBuffer)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    srcImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(srcImage)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}


VKAPI_ATTR void VKAPI_CALL CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstBuffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstBuffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}


VKAPI_ATTR void VKAPI_CALL CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstBuffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstBuffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}


VKAPI_ATTR void VKAPI_CALL CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['image']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(image)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}


VKAPI_ATTR void VKAPI_CALL CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['image']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(image)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}


VKAPI_ATTR void VKAPI_CALL CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstImage', 'srcImage']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstImage)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    srcImage = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(srcImage)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}


VKAPI_ATTR void VKAPI_CALL CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['event']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    event = (VkEvent)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(event)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdSetEvent(commandBuffer, event, stageMask);
}


VKAPI_ATTR void VKAPI_CALL CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['event']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    event = (VkEvent)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(event)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdResetEvent(commandBuffer, event, stageMask);
}


VKAPI_ATTR void VKAPI_CALL CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['pBufferMemoryBarriers[bufferMemoryBarrierCount]', 'pEvents[eventCount]', 'pImageMemoryBarriers[imageMemoryBarrierCount]']
//LOCAL DECLS:['pBufferMemoryBarriers', 'pEvents', 'pImageMemoryBarriers']
    VkEvent* local_pEvents = NULL;
    safe_VkBufferMemoryBarrier* local_pBufferMemoryBarriers = NULL;
    safe_VkImageMemoryBarrier* local_pImageMemoryBarriers = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pBufferMemoryBarriers) {
        local_pBufferMemoryBarriers = new safe_VkBufferMemoryBarrier[bufferMemoryBarrierCount];
        for (uint32_t idx0=0; idx0<bufferMemoryBarrierCount; ++idx0) {
            local_pBufferMemoryBarriers[idx0].initialize(&pBufferMemoryBarriers[idx0]);
            if (pBufferMemoryBarriers[idx0].buffer) {
                local_pBufferMemoryBarriers[idx0].buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBufferMemoryBarriers[idx0].buffer)];
            }
        }
    }
    if (pEvents) {
        local_pEvents = new VkEvent[eventCount];
        for (uint32_t idx1=0; idx1<eventCount; ++idx1) {
            local_pEvents[idx1] = (VkEvent)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pEvents[idx1])];
        }
    }
    if (pImageMemoryBarriers) {
        local_pImageMemoryBarriers = new safe_VkImageMemoryBarrier[imageMemoryBarrierCount];
        for (uint32_t idx2=0; idx2<imageMemoryBarrierCount; ++idx2) {
            local_pImageMemoryBarriers[idx2].initialize(&pImageMemoryBarriers[idx2]);
            if (pImageMemoryBarriers[idx2].image) {
                local_pImageMemoryBarriers[idx2].image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pImageMemoryBarriers[idx2].image)];
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdWaitEvents(commandBuffer, eventCount, (const VkEvent*)local_pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, (const VkBufferMemoryBarrier*)local_pBufferMemoryBarriers, imageMemoryBarrierCount, (const VkImageMemoryBarrier*)local_pImageMemoryBarriers);
    if (local_pBufferMemoryBarriers)
        delete[] local_pBufferMemoryBarriers;
    if (local_pEvents)
        delete[] local_pEvents;
    if (local_pImageMemoryBarriers)
        delete[] local_pImageMemoryBarriers;
}


VKAPI_ATTR void VKAPI_CALL CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['pBufferMemoryBarriers[bufferMemoryBarrierCount]', 'pImageMemoryBarriers[imageMemoryBarrierCount]']
//LOCAL DECLS:['pBufferMemoryBarriers', 'pImageMemoryBarriers']
    safe_VkBufferMemoryBarrier* local_pBufferMemoryBarriers = NULL;
    safe_VkImageMemoryBarrier* local_pImageMemoryBarriers = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pBufferMemoryBarriers) {
        local_pBufferMemoryBarriers = new safe_VkBufferMemoryBarrier[bufferMemoryBarrierCount];
        for (uint32_t idx0=0; idx0<bufferMemoryBarrierCount; ++idx0) {
            local_pBufferMemoryBarriers[idx0].initialize(&pBufferMemoryBarriers[idx0]);
            if (pBufferMemoryBarriers[idx0].buffer) {
                local_pBufferMemoryBarriers[idx0].buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pBufferMemoryBarriers[idx0].buffer)];
            }
        }
    }
    if (pImageMemoryBarriers) {
        local_pImageMemoryBarriers = new safe_VkImageMemoryBarrier[imageMemoryBarrierCount];
        for (uint32_t idx1=0; idx1<imageMemoryBarrierCount; ++idx1) {
            local_pImageMemoryBarriers[idx1].initialize(&pImageMemoryBarriers[idx1]);
            if (pImageMemoryBarriers[idx1].image) {
                local_pImageMemoryBarriers[idx1].image = (VkImage)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pImageMemoryBarriers[idx1].image)];
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, (const VkBufferMemoryBarrier*)local_pBufferMemoryBarriers, imageMemoryBarrierCount, (const VkImageMemoryBarrier*)local_pImageMemoryBarriers);
    if (local_pBufferMemoryBarriers)
        delete[] local_pBufferMemoryBarriers;
    if (local_pImageMemoryBarriers)
        delete[] local_pImageMemoryBarriers;
}


VKAPI_ATTR void VKAPI_CALL CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['queryPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    queryPool = (VkQueryPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(queryPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBeginQuery(commandBuffer, queryPool, query, flags);
}


VKAPI_ATTR void VKAPI_CALL CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['queryPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    queryPool = (VkQueryPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(queryPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdEndQuery(commandBuffer, queryPool, query);
}


VKAPI_ATTR void VKAPI_CALL CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['queryPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    queryPool = (VkQueryPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(queryPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}


VKAPI_ATTR void VKAPI_CALL CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['queryPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    queryPool = (VkQueryPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(queryPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}


VKAPI_ATTR void VKAPI_CALL CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['dstBuffer', 'queryPool']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    dstBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(dstBuffer)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    queryPool = (VkQueryPool)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(queryPool)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}


VKAPI_ATTR void VKAPI_CALL CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['layout']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    layout = (VkPipelineLayout)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(layout)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}


VKAPI_ATTR void VKAPI_CALL CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents)
{
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
    // STRUCT USES:['pRenderPassBegin']
    //LOCAL DECLS:['pRenderPassBegin']
    safe_VkRenderPassBeginInfo* local_pRenderPassBegin = NULL;
    {
        std::lock_guard<std::mutex> lock(global_lock);
        if (pRenderPassBegin) {
            local_pRenderPassBegin = new safe_VkRenderPassBeginInfo(pRenderPassBegin);
            // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
            local_pRenderPassBegin->framebuffer = (VkFramebuffer)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pRenderPassBegin->framebuffer)];
            // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
            local_pRenderPassBegin->renderPass = (VkRenderPass)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pRenderPassBegin->renderPass)];
        }
    }
    // CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdBeginRenderPass(commandBuffer, (const VkRenderPassBeginInfo*)local_pRenderPassBegin, contents);
    if (local_pRenderPassBegin)
        delete local_pRenderPassBegin;
}


VKAPI_ATTR void VKAPI_CALL DestroySurfaceKHR(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);
// STRUCT USES:['surface']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_surface = reinterpret_cast<uint64_t &>(surface);
    surface = (VkSurfaceKHR)my_map_data->unique_id_mapping[local_surface];
    my_map_data->unique_id_mapping.erase(local_surface);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->instance_dispatch_table->DestroySurfaceKHR(instance, surface, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['surface']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    surface = (VkSurfaceKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(surface)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['surface']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    surface = (VkSurfaceKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(surface)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['surface']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    surface = (VkSurfaceKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(surface)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['surface']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    surface = (VkSurfaceKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(surface)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['swapchain']
    std::unique_lock<std::mutex> lock(global_lock);
    uint64_t local_swapchain = reinterpret_cast<uint64_t &>(swapchain);
    swapchain = (VkSwapchainKHR)my_map_data->unique_id_mapping[local_swapchain];
    my_map_data->unique_id_mapping.erase(local_swapchain);
    lock.unlock();
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->DestroySwapchainKHR(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL AcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['fence', 'semaphore', 'swapchain']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    fence = (VkFence)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(fence)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    semaphore = (VkSemaphore)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(semaphore)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    swapchain = (VkSwapchainKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(swapchain)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(queue), layer_data_map);
// STRUCT USES:['pPresentInfo']
//LOCAL DECLS:['pPresentInfo']
    safe_VkPresentInfoKHR* local_pPresentInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pPresentInfo) {
        local_pPresentInfo = new safe_VkPresentInfoKHR(pPresentInfo);
        if (local_pPresentInfo->pSwapchains) {
            for (uint32_t idx0=0; idx0<pPresentInfo->swapchainCount; ++idx0) {
                local_pPresentInfo->pSwapchains[idx0] = (VkSwapchainKHR)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pPresentInfo->pSwapchains[idx0])];
            }
        }
        if (local_pPresentInfo->pWaitSemaphores) {
            for (uint32_t idx1=0; idx1<pPresentInfo->waitSemaphoreCount; ++idx1) {
                local_pPresentInfo->pWaitSemaphores[idx1] = (VkSemaphore)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pPresentInfo->pWaitSemaphores[idx1])];
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->QueuePresentKHR(queue, (const VkPresentInfoKHR*)local_pPresentInfo);
    if (local_pPresentInfo)
        delete local_pPresentInfo;
    return result;
}


#ifdef VK_USE_PLATFORM_WIN32_KHR
VKAPI_ATTR VkResult VKAPI_CALL CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pSurface);
        *pSurface = reinterpret_cast<VkSurfaceKHR&>(unique_id);
    }
    return result;
}
#endif

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceDisplayPlanePropertiesKHR(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['pProperties[pPropertyCount]']
//LOCAL DECLS:['pProperties']
    safe_VkDisplayPlanePropertiesKHR* local_pProperties = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pProperties) {
        local_pProperties = new safe_VkDisplayPlanePropertiesKHR[*pPropertyCount];
        for (uint32_t idx0=0; idx0<*pPropertyCount; ++idx0) {
            local_pProperties[idx0].initialize(&pProperties[idx0]);
            if (pProperties[idx0].currentDisplay) {
                local_pProperties[idx0].currentDisplay = (VkDisplayKHR)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pProperties[idx0].currentDisplay)];
            }
        }
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, ( VkDisplayPlanePropertiesKHR*)local_pProperties);
    if (local_pProperties)
        delete[] local_pProperties;
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDisplayModeKHR(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['display']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    display = (VkDisplayKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(display)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pMode);
        *pMode = reinterpret_cast<VkDisplayModeKHR&>(unique_id);
    }
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetDisplayPlaneCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(physicalDevice), layer_data_map);
// STRUCT USES:['mode']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    mode = (VkDisplayModeKHR)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(mode)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateDisplayPlaneSurfaceKHR(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(instance), layer_data_map);
// STRUCT USES:['pCreateInfo']
//LOCAL DECLS:['pCreateInfo']
    safe_VkDisplaySurfaceCreateInfoKHR* local_pCreateInfo = NULL;
    {
    std::lock_guard<std::mutex> lock(global_lock);
    if (pCreateInfo) {
        local_pCreateInfo = new safe_VkDisplaySurfaceCreateInfoKHR(pCreateInfo);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
        local_pCreateInfo->displayMode = (VkDisplayModeKHR)my_map_data->unique_id_mapping[reinterpret_cast<const uint64_t &>(pCreateInfo->displayMode)];
    }
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->instance_dispatch_table->CreateDisplayPlaneSurfaceKHR(instance, (const VkDisplaySurfaceCreateInfoKHR*)local_pCreateInfo, pAllocator, pSurface);
    if (local_pCreateInfo)
        delete local_pCreateInfo;
    if (VK_SUCCESS == result) {
        std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #961
        uint64_t unique_id = global_unique_id++;
        my_map_data->unique_id_mapping[unique_id] = reinterpret_cast<uint64_t &>(*pSurface);
        *pSurface = reinterpret_cast<VkSurfaceKHR&>(unique_id);
    }
    return result;
}


VKAPI_ATTR void VKAPI_CALL CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['buffer', 'countBuffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    countBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(countBuffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


VKAPI_ATTR void VKAPI_CALL CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(commandBuffer), layer_data_map);
// STRUCT USES:['buffer', 'countBuffer']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    buffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(buffer)];
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    countBuffer = (VkBuffer)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(countBuffer)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    my_map_data->device_dispatch_table->CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}


VKAPI_ATTR VkResult VKAPI_CALL GetMemoryWin32HandleNV(VkDevice device, VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle)
{
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #883
    layer_data *my_map_data = get_my_data_ptr(get_dispatch_key(device), layer_data_map);
// STRUCT USES:['memory']
    {
    std::lock_guard<std::mutex> lock(global_lock);
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #842
    memory = (VkDeviceMemory)my_map_data->unique_id_mapping[reinterpret_cast<uint64_t &>(memory)];
    }
// CODEGEN : file C:/dev/xgl/Vulkan-LoaderAndValidationLayers/vk-layer-generate.py line #980
    VkResult result = my_map_data->device_dispatch_table->GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
    return result;
}


// intercepts
struct { const char* name; PFN_vkVoidFunction pFunc; } procmap[] = {
    { "vkCreateInstance", reinterpret_cast<PFN_vkVoidFunction>(CreateInstance) },
    { "vkDestroyInstance", reinterpret_cast<PFN_vkVoidFunction>(DestroyInstance) },
//  { "vkEnumeratePhysicalDevices", reinterpret_cast<PFN_vkVoidFunction>(EnumeratePhysicalDevices) },
    { "vkGetInstanceProcAddr", reinterpret_cast<PFN_vkVoidFunction>(GetInstanceProcAddr) },
    { "vkGetDeviceProcAddr", reinterpret_cast<PFN_vkVoidFunction>(GetDeviceProcAddr) },
    { "vkCreateDevice", reinterpret_cast<PFN_vkVoidFunction>(CreateDevice) },
    { "vkDestroyDevice", reinterpret_cast<PFN_vkVoidFunction>(DestroyDevice) },
//  { "vkGetDeviceQueue", reinterpret_cast<PFN_vkVoidFunction>(GetDeviceQueue) },
    { "vkQueueSubmit", reinterpret_cast<PFN_vkVoidFunction>(QueueSubmit) },
//  { "vkQueueWaitIdle", reinterpret_cast<PFN_vkVoidFunction>(QueueWaitIdle) },
//  { "vkDeviceWaitIdle", reinterpret_cast<PFN_vkVoidFunction>(DeviceWaitIdle) },
    { "vkAllocateMemory", reinterpret_cast<PFN_vkVoidFunction>(AllocateMemory) },
    { "vkFreeMemory", reinterpret_cast<PFN_vkVoidFunction>(FreeMemory) },
    { "vkMapMemory", reinterpret_cast<PFN_vkVoidFunction>(MapMemory) },
    { "vkUnmapMemory", reinterpret_cast<PFN_vkVoidFunction>(UnmapMemory) },
    { "vkFlushMappedMemoryRanges", reinterpret_cast<PFN_vkVoidFunction>(FlushMappedMemoryRanges) },
    { "vkInvalidateMappedMemoryRanges", reinterpret_cast<PFN_vkVoidFunction>(InvalidateMappedMemoryRanges) },
    { "vkGetDeviceMemoryCommitment", reinterpret_cast<PFN_vkVoidFunction>(GetDeviceMemoryCommitment) },
    { "vkBindBufferMemory", reinterpret_cast<PFN_vkVoidFunction>(BindBufferMemory) },
    { "vkBindImageMemory", reinterpret_cast<PFN_vkVoidFunction>(BindImageMemory) },
    { "vkGetBufferMemoryRequirements", reinterpret_cast<PFN_vkVoidFunction>(GetBufferMemoryRequirements) },
    { "vkGetImageMemoryRequirements", reinterpret_cast<PFN_vkVoidFunction>(GetImageMemoryRequirements) },
    { "vkGetImageSparseMemoryRequirements", reinterpret_cast<PFN_vkVoidFunction>(GetImageSparseMemoryRequirements) },
    { "vkQueueBindSparse", reinterpret_cast<PFN_vkVoidFunction>(QueueBindSparse) },
    { "vkCreateFence", reinterpret_cast<PFN_vkVoidFunction>(CreateFence) },
    { "vkDestroyFence", reinterpret_cast<PFN_vkVoidFunction>(DestroyFence) },
    { "vkResetFences", reinterpret_cast<PFN_vkVoidFunction>(ResetFences) },
    { "vkGetFenceStatus", reinterpret_cast<PFN_vkVoidFunction>(GetFenceStatus) },
    { "vkWaitForFences", reinterpret_cast<PFN_vkVoidFunction>(WaitForFences) },
    { "vkCreateSemaphore", reinterpret_cast<PFN_vkVoidFunction>(CreateSemaphore) },
    { "vkDestroySemaphore", reinterpret_cast<PFN_vkVoidFunction>(DestroySemaphore) },
    { "vkCreateEvent", reinterpret_cast<PFN_vkVoidFunction>(CreateEvent) },
    { "vkDestroyEvent", reinterpret_cast<PFN_vkVoidFunction>(DestroyEvent) },
    { "vkGetEventStatus", reinterpret_cast<PFN_vkVoidFunction>(GetEventStatus) },
    { "vkSetEvent", reinterpret_cast<PFN_vkVoidFunction>(SetEvent) },
    { "vkResetEvent", reinterpret_cast<PFN_vkVoidFunction>(ResetEvent) },
    { "vkCreateQueryPool", reinterpret_cast<PFN_vkVoidFunction>(CreateQueryPool) },
    { "vkDestroyQueryPool", reinterpret_cast<PFN_vkVoidFunction>(DestroyQueryPool) },
    { "vkGetQueryPoolResults", reinterpret_cast<PFN_vkVoidFunction>(GetQueryPoolResults) },
    { "vkCreateBuffer", reinterpret_cast<PFN_vkVoidFunction>(CreateBuffer) },
    { "vkDestroyBuffer", reinterpret_cast<PFN_vkVoidFunction>(DestroyBuffer) },
    { "vkCreateBufferView", reinterpret_cast<PFN_vkVoidFunction>(CreateBufferView) },
    { "vkDestroyBufferView", reinterpret_cast<PFN_vkVoidFunction>(DestroyBufferView) },
    { "vkCreateImage", reinterpret_cast<PFN_vkVoidFunction>(CreateImage) },
    { "vkDestroyImage", reinterpret_cast<PFN_vkVoidFunction>(DestroyImage) },
    { "vkGetImageSubresourceLayout", reinterpret_cast<PFN_vkVoidFunction>(GetImageSubresourceLayout) },
    { "vkCreateImageView", reinterpret_cast<PFN_vkVoidFunction>(CreateImageView) },
    { "vkDestroyImageView", reinterpret_cast<PFN_vkVoidFunction>(DestroyImageView) },
    { "vkCreateShaderModule", reinterpret_cast<PFN_vkVoidFunction>(CreateShaderModule) },
    { "vkDestroyShaderModule", reinterpret_cast<PFN_vkVoidFunction>(DestroyShaderModule) },
    { "vkCreatePipelineCache", reinterpret_cast<PFN_vkVoidFunction>(CreatePipelineCache) },
    { "vkDestroyPipelineCache", reinterpret_cast<PFN_vkVoidFunction>(DestroyPipelineCache) },
    { "vkGetPipelineCacheData", reinterpret_cast<PFN_vkVoidFunction>(GetPipelineCacheData) },
    { "vkMergePipelineCaches", reinterpret_cast<PFN_vkVoidFunction>(MergePipelineCaches) },
    { "vkCreateGraphicsPipelines", reinterpret_cast<PFN_vkVoidFunction>(CreateGraphicsPipelines) },
    { "vkCreateComputePipelines", reinterpret_cast<PFN_vkVoidFunction>(CreateComputePipelines) },
    { "vkDestroyPipeline", reinterpret_cast<PFN_vkVoidFunction>(DestroyPipeline) },
    { "vkCreatePipelineLayout", reinterpret_cast<PFN_vkVoidFunction>(CreatePipelineLayout) },
    { "vkDestroyPipelineLayout", reinterpret_cast<PFN_vkVoidFunction>(DestroyPipelineLayout) },
    { "vkCreateSampler", reinterpret_cast<PFN_vkVoidFunction>(CreateSampler) },
    { "vkDestroySampler", reinterpret_cast<PFN_vkVoidFunction>(DestroySampler) },
    { "vkCreateDescriptorSetLayout", reinterpret_cast<PFN_vkVoidFunction>(CreateDescriptorSetLayout) },
    { "vkDestroyDescriptorSetLayout", reinterpret_cast<PFN_vkVoidFunction>(DestroyDescriptorSetLayout) },
    { "vkCreateDescriptorPool", reinterpret_cast<PFN_vkVoidFunction>(CreateDescriptorPool) },
    { "vkDestroyDescriptorPool", reinterpret_cast<PFN_vkVoidFunction>(DestroyDescriptorPool) },
    { "vkResetDescriptorPool", reinterpret_cast<PFN_vkVoidFunction>(ResetDescriptorPool) },
    { "vkAllocateDescriptorSets", reinterpret_cast<PFN_vkVoidFunction>(AllocateDescriptorSets) },
    { "vkFreeDescriptorSets", reinterpret_cast<PFN_vkVoidFunction>(FreeDescriptorSets) },
    { "vkUpdateDescriptorSets", reinterpret_cast<PFN_vkVoidFunction>(UpdateDescriptorSets) },
    { "vkCreateFramebuffer", reinterpret_cast<PFN_vkVoidFunction>(CreateFramebuffer) },
    { "vkDestroyFramebuffer", reinterpret_cast<PFN_vkVoidFunction>(DestroyFramebuffer) },
    { "vkCreateRenderPass", reinterpret_cast<PFN_vkVoidFunction>(CreateRenderPass) },
    { "vkDestroyRenderPass", reinterpret_cast<PFN_vkVoidFunction>(DestroyRenderPass) },
    { "vkGetRenderAreaGranularity", reinterpret_cast<PFN_vkVoidFunction>(GetRenderAreaGranularity) },
    { "vkCreateCommandPool", reinterpret_cast<PFN_vkVoidFunction>(CreateCommandPool) },
    { "vkDestroyCommandPool", reinterpret_cast<PFN_vkVoidFunction>(DestroyCommandPool) },
    { "vkResetCommandPool", reinterpret_cast<PFN_vkVoidFunction>(ResetCommandPool) },
    { "vkAllocateCommandBuffers", reinterpret_cast<PFN_vkVoidFunction>(AllocateCommandBuffers) },
    { "vkFreeCommandBuffers", reinterpret_cast<PFN_vkVoidFunction>(FreeCommandBuffers) },
    { "vkBeginCommandBuffer", reinterpret_cast<PFN_vkVoidFunction>(BeginCommandBuffer) },
//  { "vkEndCommandBuffer", reinterpret_cast<PFN_vkVoidFunction>(EndCommandBuffer) },
//  { "vkResetCommandBuffer", reinterpret_cast<PFN_vkVoidFunction>(ResetCommandBuffer) },
    { "vkCmdBindPipeline", reinterpret_cast<PFN_vkVoidFunction>(CmdBindPipeline) },
//  { "vkCmdSetViewport", reinterpret_cast<PFN_vkVoidFunction>(CmdSetViewport) },
//  { "vkCmdSetScissor", reinterpret_cast<PFN_vkVoidFunction>(CmdSetScissor) },
//  { "vkCmdSetLineWidth", reinterpret_cast<PFN_vkVoidFunction>(CmdSetLineWidth) },
//  { "vkCmdSetDepthBias", reinterpret_cast<PFN_vkVoidFunction>(CmdSetDepthBias) },
//  { "vkCmdSetBlendConstants", reinterpret_cast<PFN_vkVoidFunction>(CmdSetBlendConstants) },
//  { "vkCmdSetDepthBounds", reinterpret_cast<PFN_vkVoidFunction>(CmdSetDepthBounds) },
//  { "vkCmdSetStencilCompareMask", reinterpret_cast<PFN_vkVoidFunction>(CmdSetStencilCompareMask) },
//  { "vkCmdSetStencilWriteMask", reinterpret_cast<PFN_vkVoidFunction>(CmdSetStencilWriteMask) },
//  { "vkCmdSetStencilReference", reinterpret_cast<PFN_vkVoidFunction>(CmdSetStencilReference) },
    { "vkCmdBindDescriptorSets", reinterpret_cast<PFN_vkVoidFunction>(CmdBindDescriptorSets) },
    { "vkCmdBindIndexBuffer", reinterpret_cast<PFN_vkVoidFunction>(CmdBindIndexBuffer) },
    { "vkCmdBindVertexBuffers", reinterpret_cast<PFN_vkVoidFunction>(CmdBindVertexBuffers) },
//  { "vkCmdDraw", reinterpret_cast<PFN_vkVoidFunction>(CmdDraw) },
//  { "vkCmdDrawIndexed", reinterpret_cast<PFN_vkVoidFunction>(CmdDrawIndexed) },
    { "vkCmdDrawIndirect", reinterpret_cast<PFN_vkVoidFunction>(CmdDrawIndirect) },
    { "vkCmdDrawIndexedIndirect", reinterpret_cast<PFN_vkVoidFunction>(CmdDrawIndexedIndirect) },
//  { "vkCmdDispatch", reinterpret_cast<PFN_vkVoidFunction>(CmdDispatch) },
    { "vkCmdDispatchIndirect", reinterpret_cast<PFN_vkVoidFunction>(CmdDispatchIndirect) },
    { "vkCmdCopyBuffer", reinterpret_cast<PFN_vkVoidFunction>(CmdCopyBuffer) },
    { "vkCmdCopyImage", reinterpret_cast<PFN_vkVoidFunction>(CmdCopyImage) },
    { "vkCmdBlitImage", reinterpret_cast<PFN_vkVoidFunction>(CmdBlitImage) },
    { "vkCmdCopyBufferToImage", reinterpret_cast<PFN_vkVoidFunction>(CmdCopyBufferToImage) },
    { "vkCmdCopyImageToBuffer", reinterpret_cast<PFN_vkVoidFunction>(CmdCopyImageToBuffer) },
    { "vkCmdUpdateBuffer", reinterpret_cast<PFN_vkVoidFunction>(CmdUpdateBuffer) },
    { "vkCmdFillBuffer", reinterpret_cast<PFN_vkVoidFunction>(CmdFillBuffer) },
    { "vkCmdClearColorImage", reinterpret_cast<PFN_vkVoidFunction>(CmdClearColorImage) },
    { "vkCmdClearDepthStencilImage", reinterpret_cast<PFN_vkVoidFunction>(CmdClearDepthStencilImage) },
//  { "vkCmdClearAttachments", reinterpret_cast<PFN_vkVoidFunction>(CmdClearAttachments) },
    { "vkCmdResolveImage", reinterpret_cast<PFN_vkVoidFunction>(CmdResolveImage) },
    { "vkCmdSetEvent", reinterpret_cast<PFN_vkVoidFunction>(CmdSetEvent) },
    { "vkCmdResetEvent", reinterpret_cast<PFN_vkVoidFunction>(CmdResetEvent) },
    { "vkCmdWaitEvents", reinterpret_cast<PFN_vkVoidFunction>(CmdWaitEvents) },
    { "vkCmdPipelineBarrier", reinterpret_cast<PFN_vkVoidFunction>(CmdPipelineBarrier) },
    { "vkCmdBeginQuery", reinterpret_cast<PFN_vkVoidFunction>(CmdBeginQuery) },
    { "vkCmdEndQuery", reinterpret_cast<PFN_vkVoidFunction>(CmdEndQuery) },
    { "vkCmdResetQueryPool", reinterpret_cast<PFN_vkVoidFunction>(CmdResetQueryPool) },
    { "vkCmdWriteTimestamp", reinterpret_cast<PFN_vkVoidFunction>(CmdWriteTimestamp) },
    { "vkCmdCopyQueryPoolResults", reinterpret_cast<PFN_vkVoidFunction>(CmdCopyQueryPoolResults) },
    { "vkCmdPushConstants", reinterpret_cast<PFN_vkVoidFunction>(CmdPushConstants) },
    { "vkCmdBeginRenderPass", reinterpret_cast<PFN_vkVoidFunction>(CmdBeginRenderPass) },
//  { "vkCmdNextSubpass", reinterpret_cast<PFN_vkVoidFunction>(CmdNextSubpass) },
//  { "vkCmdEndRenderPass", reinterpret_cast<PFN_vkVoidFunction>(CmdEndRenderPass) },
//  { "vkCmdExecuteCommands", reinterpret_cast<PFN_vkVoidFunction>(CmdExecuteCommands) },
//  { "vkCreateDebugReportCallbackEXT", reinterpret_cast<PFN_vkVoidFunction>(CreateDebugReportCallbackEXT) },
//  { "vkDestroyDebugReportCallbackEXT", reinterpret_cast<PFN_vkVoidFunction>(DestroyDebugReportCallbackEXT) },
//  { "vkDebugReportMessageEXT", reinterpret_cast<PFN_vkVoidFunction>(DebugReportMessageEXT) },
    { "vkCmdDrawIndirectCountAMD", reinterpret_cast<PFN_vkVoidFunction>(CmdDrawIndirectCountAMD) },
    { "vkCmdDrawIndexedIndirectCountAMD", reinterpret_cast<PFN_vkVoidFunction>(CmdDrawIndexedIndirectCountAMD) },
#ifdef VK_USE_PLATFORM_WIN32_KHR
    { "vkGetMemoryWin32HandleNV", reinterpret_cast<PFN_vkVoidFunction>(GetMemoryWin32HandleNV) },
#endif
};


} // namespace unique_objects_wrappers

#endif
