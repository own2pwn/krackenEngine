/** @file MRKSwapchain.cpp
*
*  @brief mrk::Swapchain implementation
*
*  @author Juan Ramos
*/
 #include "MRKSwapchain.h"
#include "MRKDevice.h"
#include "MRKWindowSystem.h"
#include "MRKVulkanTools.h"
#include "MRKImage.h"
#include <array>
#include "MRKGraphicsSystem.h"

namespace
{
    vk::ClearColorValue const clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    vk::ClearDepthStencilValue const clearDepth = { 1.0f, 0 };

    std::array<vk::ClearValue, 2> const clearValues = { clearColor, clearDepth };
}

namespace mrk
{
    void Swapchain::cleanUp()
    {
        auto &logicalDevice = g_graphicsSystemSingleton.device.logicalDevice_; // Just to write less code

		depthImage_.destroy();

        for (auto & frameBuffer : swapchainFramebuffers_)
        {
            logicalDevice.destroyFramebuffer(frameBuffer);
        }

        for (auto & swapChainImageView : swapChainImageViews_)
        {
            logicalDevice.destroyImageView(swapChainImageView);
        }

        logicalDevice.destroySwapchainKHR(swapChain_);
    }

    void Swapchain::createMRKSwapChain()
    {
        createSwapChain();
        createImageViews();
        createDepthResources();
        createRenderPass();
        createFrameBuffers();
    }

    void Swapchain::createSwapChain()
    {
        auto const &device = g_graphicsSystemSingleton.device;
        auto const &surface = g_graphicsSystemSingleton.surface;

        // If true then the family supports graphics and presentation
        // http://vulkan.gpuinfo.org/displayreport.php?id=1706#queuefamilies
        bool familiesAreConcurrent = device.queueFamilyIndices_.graphicsFamilyIndex == device.queueFamilyIndices_.presentFamilyIndex;
        uint32_t queueFamilyIndices[] = {
            static_cast<uint32_t>(device.queueFamilyIndices_.graphicsFamilyIndex),
            static_cast<uint32_t>(device.queueFamilyIndices_.graphicsFamilyIndex)
        };

        auto createInfo = vk::SwapchainCreateInfoKHR()
            .setSurface(surface)
            .setMinImageCount(maxImageCount_)
            .setImageFormat(swapChainSurfaceFormat_.format)
            .setImageColorSpace(swapChainSurfaceFormat_.colorSpace)
            .setImageExtent(swapChainExtent_)
            .setPresentMode(presentMode_)
            // This is always gonna be 1 for us unless you are developing a stereoscopic application
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            // For example on tablets the current transform might not be just the identity matrix
            .setPreTransform(swapChainSupport_.capabilities.currentTransform)
            // Specifies if the alpha channel should be used for blending with other windows in the window
            // system. Almost always this should be ignored. COOOOOOL
            // But currently this isn't supported
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            // If true that means that we don't care about the color of pixels that are obscured.
            // EX: Another window is in front. This can be a problem if those pixels need to be read back
            .setClipped(true);

        if (familiesAreConcurrent == false)
        {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }

        auto &logicalDevice = device.logicalDevice_;
        MRK_CATCH(swapChain_ = logicalDevice.createSwapchainKHR(createInfo))
        MRK_CATCH(swapChainImages_ = logicalDevice.getSwapchainImagesKHR(swapChain_))
    }

    void Swapchain::createImageViews()
    {
        auto viewInfo = vk::ImageViewCreateInfo()
            .setViewType(vk::ImageViewType::e2DArray)
            .setFormat(swapChainSurfaceFormat_.format)
            .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

        swapChainImageViews_.clear();

        vk::ImageView tempView;
        for (auto const & image : swapChainImages_)
        {
            viewInfo.image = image;

            MRK_CATCH(tempView = g_graphicsSystemSingleton.device.logicalDevice_.createImageView(viewInfo))

            swapChainImageViews_.push_back(tempView);
        }
    }

    void Swapchain::createDepthResources() const
    {
        /*
        * Creating a depth image is fairly straightforward. It should have the same resolution as the color attachment, defined by
        * the swap chain extent, an image usage appropriate for a depth attachment, optimal tiling and device local memory. The only
        * question is: what is the right format for a depth image? The format must contain a depth component, indicated by _D??_ in
        * the VK_FORMAT_.
        */

		// transition its layout
		depthImage_.transitionLayout(depthFormat_, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal,
			                   g_graphicsSystemSingleton.graphicsPool, g_graphicsSystemSingleton.graphicsQueue);
    }

    void Swapchain::reCreateDepthResources()
    {
    mrk::Image::CreateInfo createInfo = { swapChainExtent_.width,
                 swapChainExtent_.height,
                 depthFormat_,
                 vk::ImageTiling::eOptimal,
                 vk::ImageUsageFlagBits::eDepthStencilAttachment,
                 vk::MemoryPropertyFlagBits::eDeviceLocal,
                 vk::ImageAspectFlagBits::eDepth};

        depthImage_.info_ = createInfo;
        depthImage_.createImage(createInfo);
        depthImage_.createImageView(createInfo.format, createInfo.aspectFlags);

        createDepthResources();
    }

    void Swapchain::createRenderPass()
    {
        vk::AttachmentDescription colorAttachment = vk::AttachmentDescription()
            .setFormat(swapChainSurfaceFormat_.format)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

        vk::AttachmentDescription depthAttachment = vk::AttachmentDescription()
            .setFormat(depthFormat_)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::AttachmentReference colorAttachmentReference = vk::AttachmentReference()
            .setAttachment(0)
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        vk::AttachmentReference depthAttachmentReference = vk::AttachmentReference()
            .setAttachment(1)
            .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::SubpassDescription subpass = vk::SubpassDescription()
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)
            .setPColorAttachments(&colorAttachmentReference)
            .setPDepthStencilAttachment(&depthAttachmentReference);

        // wait for the color attachment output of the source subpass
        // wait on this before doing any reads or writes in the
        // color attachment output stage of the destination subpass
        vk::SubpassDependency dependency = vk::SubpassDependency()
            .setSrcSubpass(VK_SUBPASS_EXTERNAL)
            .setDstSubpass(0)
            .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
            // there is no mask to do nothing! .setSrcAccessMask()
            .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
            .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

        std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        vk::RenderPassCreateInfo renderPassCreateInfo = vk::RenderPassCreateInfo()
            .setSubpassCount(1)
            .setPSubpasses(&subpass)
            .setDependencyCount(1)
            .setPDependencies(&dependency)
            .setAttachmentCount(static_cast<uint32_t>(attachments.size()))
            .setPAttachments(attachments.data());

        MRK_CATCH(renderPass_ = g_graphicsSystemSingleton.device.logicalDevice_.createRenderPass(renderPassCreateInfo));

		renderBeginPassInfo_.renderPass = renderPass_;
    }

    void Swapchain::createFrameBuffers()
    {
        swapchainFramebuffers_.resize(swapChainImageViews_.size());
        std::array<vk::ImageView, 2> attachements;

        for (size_t i = 0; i < swapchainFramebuffers_.size(); ++i)
        {
            attachements[0] = swapChainImageViews_[i];
            attachements[1] = depthImage_.mImageView;

            auto framebufferInfo = vk::FramebufferCreateInfo()
                .setHeight(swapChainExtent_.height)
                .setWidth(swapChainExtent_.width)
                .setRenderPass(renderPass_)
                .setAttachmentCount(static_cast<uint32_t>(attachements.size()))
                .setPAttachments(attachements.data())
                .setLayers(1);

            MRK_CATCH(swapchainFramebuffers_[i] = g_graphicsSystemSingleton.device.logicalDevice_.createFramebuffer(framebufferInfo))
        }
    }

    /**
     * \brief 
     *  Gets the surface capabilities, formats, and presentation modes supported
     * \return 
     *  Returns a variable carrying the surface capabilities, formats, and presentation modes supported
     */
    Swapchain::SwapChainSupportDetails Swapchain::getSwapChainSupportDetails() const
    {
        SwapChainSupportDetails details;
        auto const & device = g_graphicsSystemSingleton.device;
        auto const & surface = g_graphicsSystemSingleton.surface;

        device.physicalDevice_.getSurfaceCapabilitiesKHR(surface, &details.capabilities);

        details.formats = device.physicalDevice_.getSurfaceFormatsKHR(surface);
        details.presentModes = device.physicalDevice_.getSurfacePresentModesKHR(surface);

        return details;
    }

    /**
     * \brief 
     *  Choose the format for the swap chain. The ideal format is:
     *      vk::SurfaceFormatKHR{ vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear } 
     *  
     * eB8G8R8A8Unorm, means that we store the B, G, R and alpha channels in that order
     * with an 8 bit unsigned integer for a total of 32 bits per pixel. The colorSpace member indicates
     * if the SRGB color space is supported or not using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR flag. Note
     * that this flag used to be called eSrgbNonlinear in old versions of the specification.
     *
     * For the color space we'll use SRGB if it is available, because it results in more accurate perceived colors.
     * Working directly with SRGB colors is a little bit challenging, so we'll use standard RGB for the color format,
     * of which one of the most common ones is eB8G8R8A8Unorm.
     *  
     * \return
     *  The most desired format possible
     */
    vk::SurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat() const
    {
        std::vector<vk::SurfaceFormatKHR> const & availableFormats = swapChainSupport_.formats;
        vk::SurfaceFormatKHR surfaceFormat = availableFormats[0]; // If the below checks fail best to rank formats but for now just choose the first one

        // Best case scenario is that the surface has no preferred format
        // Which Vulkan indicates by only returning one vkSurfaceFormat entry, and its format member set to VK_FORMAT_UNDEFINED
        if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined )
        {

            surfaceFormat = vk::SurfaceFormatKHR{ vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
        }
        else
        {
            // If we aren't free to choose, then we see if are preferred combo is listed
            for (const auto& availableFormat : availableFormats)
            {
                if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
                    availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                {
                    surfaceFormat = availableFormat;
                }
            }
        }

        return surfaceFormat;
    }

    /**
     * \brief 
     *  The swap extent is the resolution of the swap chain images and it's almost always exactly equal 
     *  to the resolution of the window that we're drawing to. The range of the possible resolutions is 
     *  defined in the VkSurfaceCapabilitiesKHR structure. Vulkan tells us to match the resolution of 
     *  the window by setting the width and height in the currentExtent member. However, some window 
     *  managers do allow us to differ here and this is indicated by setting the width and height in 
     *  currentExtent to a special value: the maximum value of uint32_t. In that case we'll pick the 
     *  resolution that best matches the window within the minImageExtent and maxImageExtent bounds.
     *  
     * \return 
     *  Either return the current extent as specified by the window system or Vulkan doesn't support that size
     *  so it will change it.
     */
    vk::Extent2D Swapchain::chooseSwapExtent() const
    {
        vk::SurfaceCapabilitiesKHR const & capabilities = swapChainSupport_.capabilities;
        vk::Extent2D actualExtent = capabilities.currentExtent;

        // This means the current extent has the ability to be smaller or bigger than it currently is
        if (capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
        {
            glm::ivec2 windowSize = g_graphicsSystemSingleton.windowSystem.getWindowSize();
            actualExtent = vk::Extent2D{ static_cast<uint32_t>(windowSize.x), static_cast<uint32_t>(windowSize.y) };

            // The max and min functions are used here to clamp the value of WIDTH and HEIGHT between the allowed minimum and 
            // maximum extents that are supported by the implementation. Make sure to include the <algorithm> header to use them.
            actualExtent.width = std::max(capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }

        return actualExtent;
    }

    /**
     * \brief
     *  Gets the best presentation mode available to us 
     *  
     *  Arguably the most important setting for the swap chain, because it represents the actual conditions
     *  for showing images to the screen.
     *
     *  Vertical Blank: The moment that the display is refreshed. Basically when it starts updating
     *  the top row of pixels.
     *
     *  SETTINGS
     *
     *  IMMEDIATE: Images submitted by application are transferred to the screen right away
     *
     *  FIFO: The swap chain is a queue where the display takes an image from the front of the queue on a
     *  vertical blank and the program inserts rendered images to the back of the queue. If the queue is full
     *  then the program has to wait. Similar to vertical-sync found in most games
     *
     *  FIFO_RELAXED: Slightly different from FIFO. If the application is late and the queue was empty
     *  at the last vertical blank, then instead of waiting for the next vertical blank the image is
     *  transferred right away. Which can result in tearing.
     *
     *  MAILBOX: Variation of FIFO instead of blocking the application when the queue is full, images that are already
     *  queued are simply replaced with the newer ones. This mode can be used to implement triple buffering, which allows you
     *  to avoid tearing with significantly less tearing issues than standard vertical sync that uses double buffering
     *  
     * \return
     *  The best available presentation mode. In this preferred order Mailbox, Immediate, Fifo.
     */
    vk::PresentModeKHR Swapchain::chooseSwapPresentMode() const
    {
        vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;  // This is the only guaranteed mode available. So in case all else fails

        // We prefer the mailbox method because of its benefits
        for (auto const &availablePresentMode : swapChainSupport_.presentModes)
        {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox)
            {
                bestMode = availablePresentMode;
                break;
            }

            if (availablePresentMode == vk::PresentModeKHR::eImmediate)
            {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }

    /**
     * \brief
     *  Returns the number of images in the swap chain, essentially the queue length. The implementation specifies the minimum amount 
     *  of images to function properly and we'll try to have one more than that to properly implement triple buffering.
     * \return
     *  The number of images that the system can support but ideally we want 3.
     */
    uint32_t Swapchain::chooseMaxImageCount() const
    {
        vk::SurfaceCapabilitiesKHR const &capabilities = swapChainSupport_.capabilities;

        // We need to decide the number of images in the swap chain, IE: The queue length
        // minImageCount is the minimum number of images the specified device supports for a swapchain created for the surface, and will be at least one.
        uint32_t maxImageCount = capabilities.minImageCount + 1;

        // A value of 0 means that there is no limit besides memory requirements, which is why we need to
        // check for that
        if (capabilities.maxImageCount > 0 &&
            maxImageCount > capabilities.maxImageCount)
        {
            maxImageCount = capabilities.maxImageCount;
        }

        return maxImageCount;
    }

    vk::RenderPassBeginInfo Swapchain::createRenderPassInfo() const
    {
        return vk::RenderPassBeginInfo()
            .setRenderPass(renderPass_)
            .setFramebuffer(vk::Framebuffer())
            .setRenderArea({ {0, 0}, swapChainExtent_})
            .setClearValueCount(static_cast<uint32_t>(clearValues.size()))
            .setPClearValues(clearValues.data());
    }

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
    Swapchain::Swapchain(createInfo const &info) :
        swapChainSupport_(getSwapChainSupportDetails()),
        swapChainSurfaceFormat_(chooseSwapSurfaceFormat()),
        swapChainExtent_(chooseSwapExtent()),
        presentMode_(chooseSwapPresentMode()),
        maxImageCount_(chooseMaxImageCount()),
        depthFormat_(g_graphicsSystemSingleton.device.findDepthFormat()),
		depthImage_({ swapChainExtent_.width,
				 swapChainExtent_.height,
				 depthFormat_,
				 vk::ImageTiling::eOptimal,
				 vk::ImageUsageFlagBits::eDepthStencilAttachment,
				 vk::MemoryPropertyFlagBits::eDeviceLocal,
				 vk::ImageAspectFlagBits::eDepth} ),
        renderBeginPassInfo_(createRenderPassInfo())
    {
		(void)info;
        createSwapChain();
        createImageViews();
        createDepthResources();
        createRenderPass();
        createFrameBuffers();
    }

    /**
     * \brief 
     *  Uses the logical device to destroy the vulkan objects it created
     */
    Swapchain::~Swapchain()
	{
        cleanUp();

        g_graphicsSystemSingleton.device.logicalDevice_.destroyRenderPass(renderPass_);
	}

    mrk::Swapchain & Swapchain::operator=(mrk::Swapchain && other) noexcept
    {
        swapChainSupport_ = other.swapChainSupport_;
        swapChainSurfaceFormat_ = other.swapChainSurfaceFormat_;
        swapChainExtent_ = other.swapChainExtent_;
        presentMode_ = other.presentMode_;
        maxImageCount_ = other.maxImageCount_;

        swapChain_ = other.swapChain_;
        other.swapChain_ = nullptr;

        swapChainImages_ = std::move(other.swapChainImages_);
        swapChainImageViews_ = std::move(other.swapChainImageViews_);

        renderPass_ = other.renderPass_;
        other.renderPass_ = nullptr;

        swapchainFramebuffers_ = std::move(other.swapchainFramebuffers_);

        depthFormat_ = other.depthFormat_;
        depthImage_ = std::move(other.depthImage_);

        renderBeginPassInfo_ = other.renderBeginPassInfo_;

        return *this;
    }

    /**
     * \brief 
     * If the window size changes or goes to a different monitor, etc.
     * Then the swap chain needs to be recreated.
     * 
     * This means destroying the old resources and then creating the new ones accordingly.
     */
    void Swapchain::recreate()
    {
        cleanUp();

        swapChainSupport_ = getSwapChainSupportDetails();
        swapChainSurfaceFormat_ = chooseSwapSurfaceFormat();
        swapChainExtent_ = chooseSwapExtent();
        presentMode_ = chooseSwapPresentMode();
        maxImageCount_ = chooseMaxImageCount();
        depthFormat_ = g_graphicsSystemSingleton.device.findDepthFormat();
        reCreateDepthResources();
        renderBeginPassInfo_ = createRenderPassInfo();

        createSwapChain();
        createImageViews();
        createDepthResources();
        createFrameBuffers();
    }

    /**
     * \brief
     *  When creating the command buffer all this information is provided upfront. This needs to be
     *  called since frame buffers aren't made available at all 
     * \param i
     *  The index for the desired frame buffer
     * \return
     *  A reference to the vk::RenderPassBeginInfo inside the mrk::Swapchain 
     */
    vk::RenderPassBeginInfo const& Swapchain::getRenderPassBeginInfo(int i)
    {
        renderBeginPassInfo_.framebuffer = swapchainFramebuffers_[i];

        return renderBeginPassInfo_;
    }

}
