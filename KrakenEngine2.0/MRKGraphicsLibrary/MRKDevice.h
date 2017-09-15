/** @file MRKDevice.h
*
*  @brief mrk::Device interface 
*
*  @author Juan Ramos
*/
#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

namespace mrk
{
    /**
	 * \brief 
     *  Creates and wraps the physical and logical vulkan devices. As well as wrapping 
     *  family indices and physical device properties. All done initialization so the client shouldn't need
     *  to call the needed functions
	 */
	class Device
	{
	public:
	    /**
		 * \brief 
		 *  instance in order to create the physical device
		 *  surface in order to query for if the physical device has surface support
		 *  the queues in order for creation of queues to belong to mrk::device
		 */
		struct createInfo
		{
		    createInfo(vk::Instance const& vk_instance, vk::SurfaceKHR const& surface)
		        : instance(vk_instance),
		          surface(surface)
		    {
		    }

			vk::Instance const& instance;
			vk::SurfaceKHR const &surface;
		};

	    /**
         * \brief
         *  This structure is filled by mrk::Device with the information of which families
         *  hold responsibilities for graphics and presentation
         */
        class QueueFamilyIndices
        {
        private:
            static const uint32_t NOT_FOUND = static_cast<uint32_t>(-1); // Short hand for the max value of an unsigned integer

            // mrk::Device is made a friend class since these 3 private function should not be available to the client,
            // but are needed to make life easier for constructing mrk::Device
            friend class mrk::Device;

            QueueFamilyIndices(uint32_t graphics_family, uint32_t present_family)
                : graphicsFamilyIndex(graphics_family),
                  presentFamilyIndex(present_family)
            {
            }

            static QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice const &physicalDevice, vk::SurfaceKHR const &surface);
            static bool isComplete(uint32_t graphicsFamilyIndex, uint32_t presentFamilyIndex);

        public:
            /**
             * \brief
             *  Contains the index for the family that contains graphics capability
             *  EX:
             *      Look for a flag that says GRAPHICS_BIT
             *      http://vulkan.gpuinfo.org/displayreport.php?id=1593#queuefamilies
             */
            uint32_t const graphicsFamilyIndex;
            /**
             * \brief
             *  Contains the index for the family that contains presentation capability
             *  EX:
             *      Look for supportsPresent = 1
             *      http://vulkan.gpuinfo.org/displayreport.php?id=1593#queuefamilies
             */
            uint32_t const presentFamilyIndex;
        };

	    /**
		 * \brief
		 *  Initializes the physical and logical devices. As well as device features/properties and
		 *  the "QueueFamilyIndices"
		 *  
		 * \param info
		 *  See create info struct 
		 */
		explicit Device(createInfo const& info);

	    /**
		 * \brief 
		 *  Destroy the logical device which in turn destroys the physical device
		 */
		~Device();

	    /**
         * \brief 
         *  Creates a command pool
         * \param familyIndex
         *  The family index that is used to create the pool.
         *  For instance the graphics family 
         * \return
         *  A command-pool that uses the corresponding family index.
         *  EX:
         *      A command pool for graphics
         *      A command pool for transferring 
         */
        vk::CommandPool createCommandPool(uint32_t familyIndex) const;

        /**
         * \brief 
         *   Creating a depth image is fairly straightforward. It should have the same resolution as the color attachment, defined by
         *   the swap chain extent, an image usage appropriate for a depth attachment, optimal tiling and device local memory. The only
         *   question is: what is the right format for a depth image? The format must contain a depth component, indicated by _D??_ in
         *   the VK_FORMAT_.
         *
         *   Unlike the texture image, we don't necessarily need a specific format, because we won't be directly accessing the texels from the
         *   program. It just needs to have a reasonable accuracy, at least 24 bits is common in real-world applications. There are several
         *   formats that fit this requirement:
         *       VK_FORMAT_D32_SFLOAT: 32-bit float for depth
         *       VK_FORMAT_D32_SFLOAT_S8_UINT: 32-bit signed float for depth and 8 bit stencil component
         *       VK_FORMAT_D24_UNORM_S8_UINT: 24-bit float for depth and 8 bit stencil component
         *   The stencil component is used for stencil tests, which is an additional test that can be combined with depth testing.
         *   We'll look at this in a future chapter.
         *
         *   We could simply go for the VK_FORMAT_D32_SFLOAT format, because support for it is extremely common (see the hardware database),
         *   but it's nice to add some extra flexibility to our application where possible. We're going to write a function
         *   findSupportedFormat that takes a list of candidate formats in order from most desirable
         *   to least desirable, and checks which is the first one that is supported:
         *   
         * \return
         *  Returns the best supported format
         */
        vk::Format findDepthFormat() const;
        vk::Format findSupportedFormat(std::vector<vk::Format> const &canidates, vk::ImageTiling const &tiling, vk::FormatFeatureFlagBits const & formatFeature) const; 

		vk::PhysicalDevice const physicalDevice_;
	    QueueFamilyIndices const queueFamilyIndices_;
		vk::PhysicalDeviceProperties const physicalDeviceProperties_;
		vk::PhysicalDeviceMemoryProperties const physicalDeviceMemoryProperties_;
		vk::PhysicalDeviceFeatures const physicalDeviceFeatures_;
		vk::Device const logicalDevice_;

	private:
        static vk::PhysicalDevice pickPhysicalDevice(createInfo const &info);
        vk::Device getLogicalDevice() const;
	};
}
