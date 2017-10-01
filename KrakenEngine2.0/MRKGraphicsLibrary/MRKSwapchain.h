/** @file MRKSwapchain.h
*
*  @brief mrk::Swapchain interface
*
*  @author Juan Ramos 
*/
#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include "MRKImage.h"

namespace mrk
{
    class WindowSystem;
    class Device;

    /**
     * \brief
     *  Handles all aspects of the vulkan swap chain objects.
     *  Including SwapChain objects, depth buffer, and render pass
     */
    class Swapchain
    {
	public:
		/**
		* \brief
		*  This struct is used to create the constructor for the SwapChain
		*/
		struct createInfo
		{
		};

		/**
		* \brief
		*  Creates the swap chain needed for vulkan rendering
		* \param info
		*  device: Needed for creation of the vulkan interface.
		*  surface: Needed to get the swap chain support details.
		*  command pool: Needed to create the depth resources.
		*  graphics queue: Needed to transition image layout when creating depth resources
		*  windowSystem: Needed for choosing the swap extent.
		*/
		explicit Swapchain(createInfo const &info);

        Swapchain() = default;

		/**
		* \brief
		* If the window size changes or goes to a different monitor, etc.
		* Then the swap chain needs to be recreated.
		*
		* This means destroying the old resources and then creating the new ones accordingly.
		*/
		void recreate();

		// Getters
		/**
		* \brief
		*  When creating the command buffer all this information is provided upfront. This needs to be
		*  called since frame buffers aren't made available at all
		* \param i
		*  The index for the desired frame buffer
		* \return
		*  A reference to the vk::RenderPassBeginInfo inside the mrk::Swapchain
		*/
		vk::RenderPassBeginInfo const & getRenderPassBeginInfo(int i);
		size_t getFrameBufferSize() const { return swapchainFramebuffers_.size(); }
		vk::Extent2D const &getExtent() const { return swapChainExtent_; }
		vk::SwapchainKHR const & getSwapChain() const { return swapChain_; }
		vk::RenderPass const &getRenderPass() const { return renderPass_; }

		/**
		* \brief
		*  Uses the logical device to destroy the vulkan objects it created
		*/
		~Swapchain();

    private:
        void cleanUp();

        void createMRKSwapChain();
        void createSwapChain();
        void createImageViews();
        void createDepthResources() const;
        void reCreateDepthResources();
        void createRenderPass();
        void createFrameBuffers();


        struct SwapChainSupportDetails
        {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        } swapChainSupport_;

        vk::SurfaceFormatKHR swapChainSurfaceFormat_;
        vk::Extent2D swapChainExtent_;
        vk::PresentModeKHR presentMode_;
        uint32_t maxImageCount_;

        SwapChainSupportDetails getSwapChainSupportDetails() const;
        vk::SurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat() const;
        vk::Extent2D chooseSwapExtent() const;
        vk::PresentModeKHR chooseSwapPresentMode() const;
        uint32_t chooseMaxImageCount() const;

        vk::SwapchainKHR swapChain_;
        std::vector<vk::Image> swapChainImages_;
        std::vector<vk::ImageView> swapChainImageViews_;
        vk::RenderPass renderPass_;
        std::vector<vk::Framebuffer> swapchainFramebuffers_;

		vk::Format depthFormat_;
		mrk::Image depthImage_;

        
        vk::RenderPassBeginInfo renderBeginPassInfo_;
        vk::RenderPassBeginInfo createRenderPassInfo() const;	
	};
}
