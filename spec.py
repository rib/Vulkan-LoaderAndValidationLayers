#!/usr/bin/python3 -i

import io,os,re,string,sys
import xml.etree.ElementTree as etree

spec_url = "https://www.khronos.org/registry/vulkan/specs/1.0/xhtml/vkspec.html"
error_msg_prefix = "For more information refer to Vulkan Spec Section "
ns = {'ns': 'http://www.w3.org/1999/xhtml'}

class Specification:
    def __init__(self):
        self.tree   = None
        self.val_error_dict = {} # string for enum is key that references text for output message
    def loadFile(self, file):
        """Load an API registry XML file into a Registry object and parse it"""
        self.tree = etree.parse(file)
        self.parseTree()
    def parseTree(self):
        """Parse the registry Element, once created"""
        # This must be the Element for the root <registry>
        valid_usage = False # are we under a valid usage branch?
        section_heading = '' # the text heading for the section
        section_link = '' # txt for the section html link
        unique_enum_id = 0
        self.root = self.tree.getroot()
        print "ROOT: %s" % self.root
        for child in self.root:
            print child.tag, child.attrib
        for div in self.root.iter('{http://www.w3.org/1999/xhtml}div'):
            # we're under a section
            #print "div: %s" % (div.tag)
            if div.get('class') == 'section':
                print "section: %s" % (div.attrib)
                # Find section headings and link
                for h2 in div.iter('{http://www.w3.org/1999/xhtml}h2'):
                    if h2.get('class') == 'title':
                        print "Title: %s" % (h2)
                        section_heading = "".join(h2.itertext())
                        section_link = h2[0].get('id')
                        print "Title heading (%s) & link (%s)" % (section_heading, section_link)
                for sb in div.iter('{http://www.w3.org/1999/xhtml}div'):
                    if sb.get('class') == 'sidebar':
                        for strong in sb.iter('{http://www.w3.org/1999/xhtml}strong'):
                            if strong.text == 'Valid Usage':
                                valid_usage = True
                            else:
                                valid_usage = False
                        if valid_usage:
                            for li in sb.iter('{http://www.w3.org/1999/xhtml}li'):
                                # Here is where we need to save valid usage reqs
                                error_msg_str = "%s %s which states '%s' (%s#%s)" % (error_msg_prefix, section_heading, "".join(li.itertext()), spec_url, section_link)
                                enum_str = "VALIDATION_ERROR_%s%d" % (section_heading.split()[0].replace('.', '_'), unique_enum_id)
                                unique_enum_id = unique_enum_id + 1
                                self.val_error_dict[enum_str] = error_msg_str
        print "Validation Error Dict has a total of %d unique errors and contents are:\n%s" % (unique_enum_id, self.val_error_dict)

if __name__ == "__main__":
    spec = Specification()
    spec.loadFile("vkspec.html")
    spec.parseTree()


        # <div class="sidebar">
        #   <div class="titlepage">
        #     <div>
        #       <div>
        #         <p class="title">
        #           <strong>Valid Usage</strong> # When we get to this guy, we know we're under interesting sidebar
        #         </p>
        #       </div>
        #     </div>
        #   </div>
        # <div class="itemizedlist">
        #   <ul class="itemizedlist" style="list-style-type: disc; ">
        #     <li class="listitem">
        #       <em class="parameter">
        #         <code>device</code>
        #       </em>
        #       <span class="normative">must</span> be a valid
        #       <code class="code">VkDevice</code> handle
        #     </li>
        #     <li class="listitem">
        #       <em class="parameter">
        #         <code>commandPool</code>
        #       </em>
        #       <span class="normative">must</span> be a valid
        #       <code class="code">VkCommandPool</code> handle
        #     </li>
        #     <li class="listitem">
        #       <em class="parameter">
        #         <code>flags</code>
        #       </em>
        #       <span class="normative">must</span> be a valid combination of
        #       <code class="code">
        #         <a class="link" href="#VkCommandPoolResetFlagBits">VkCommandPoolResetFlagBits</a>
        #       </code> values
        #     </li>
        #     <li class="listitem">
        #       <em class="parameter">
        #         <code>commandPool</code>
        #       </em>
        #       <span class="normative">must</span> have been created, allocated, or retrieved from
        #       <em class="parameter">
        #         <code>device</code>
        #       </em>
        #     </li>
        #     <li class="listitem">All
        #       <code class="code">VkCommandBuffer</code>
        #       objects allocated from
        #       <em class="parameter">
        #         <code>commandPool</code>
        #       </em>
        #       <span class="normative">must</span> not currently be pending execution
        #     </li>
        #   </ul>
        # </div>
        # </div>