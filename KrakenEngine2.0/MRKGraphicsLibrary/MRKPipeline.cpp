#include "Precompiled.h"
#include "MRKGraphicsSystem.h"
#include "MRKPipeline.h"
#include "MRKVulkanTools.h"
#include "mrk.h"

namespace mrk
{
    Pipeline::Pipeline() : type_(ShaderType::DEFAULT)
    {
        commandBuffers_.reserve(10);
    }

    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        layout_ = other.layout_;
        pipeline_ = other.pipeline_;
        commandBuffers_ = std::move(other.commandBuffers_);
        imageAvailable = other.imageAvailable;
        renderFinished = other.renderFinished;

        return *this;
    }

    void Pipeline::load(ShaderType type)
    {
        type_ = type;
        createSemaphores();

		const ResourceManager & resourceManager = g_graphicsSystemSingleton.resourceManager;

		// create shader stages
		//vk::PipelineShaderStageCreateInfo vertStageInfo = vk::PipelineShaderStageCreateInfo()
		//	.setStage(vk::ShaderStageFlagBits::eVertex)
		//	.setModule(resourceManager.getVertexShader())
		//	.setPName("main");

		//vk::PipelineShaderStageCreateInfo fragStageInfo = vk::PipelineShaderStageCreateInfo()
		//	.setStage(vk::ShaderStageFlagBits::eFragment)
		//	.setModule(resourceManager.getFragmentShader())
		//	.setPName("main");

		//std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = { vertStageInfo, fragStageInfo };

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        auto shaders = g_graphicsSystemSingleton.resourceManager.getShadersFromShaderType(type);
        for (std::tuple<vk::ShaderModule, vk::ShaderStageFlagBits, const char*> const &tuple : shaders)
        {
            shaderStages.emplace_back(vk::PipelineShaderStageCreateInfo()
                                        .setModule(std::get<0>(tuple))
                                        .setStage(std::get<1>(tuple))
                                        .setPName(std::get<2>(tuple))
            );
        }

		// vertex input
		vk::VertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
		std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = Vertex::getAttributeDescriptions();

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo()
			.setVertexBindingDescriptionCount(1)
			.setVertexAttributeDescriptionCount(static_cast<uint32_t>(attributeDescriptions.size()))
			.setPVertexBindingDescriptions(&bindingDescription)
			.setPVertexAttributeDescriptions(attributeDescriptions.data());

		// input assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly = vk::PipelineInputAssemblyStateCreateInfo()
			.setTopology(vk::PrimitiveTopology::eTriangleList)
			.setPrimitiveRestartEnable(VK_FALSE);

        vk::PipelineViewportStateCreateInfo viewportStateInfo = vk::PipelineViewportStateCreateInfo()
            .setViewportCount(1)
            .setScissorCount(1);

		// view port and scissor
		vk::Rect2D scissor = vk::Rect2D()
			.setOffset({ 0,0 })
			.setExtent(g_graphicsSystemSingleton.swapChain.getExtent());

		vk::Viewport viewport = vk::Viewport()
			.setX(0.0f)
			.setY(0.0f)
			.setWidth(static_cast<float>(scissor.extent.width))
			.setHeight(static_cast<float>(scissor.extent.height))
			.setMinDepth(0.0f)
			.setMaxDepth(1.0f);

        if (dynamic_ == false)
        {
            viewportStateInfo.pScissors = &scissor;
            viewportStateInfo.pViewports = &viewport;
        }

        std::array<vk::DynamicState, 2> states = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

        auto dynamicStateInfo = vk::PipelineDynamicStateCreateInfo()
            .setPDynamicStates(states.data())
            .setDynamicStateCount(static_cast<uint32_t>(states.size()));

		// rasterization state
		vk::PipelineRasterizationStateCreateInfo rasterInfo = vk::PipelineRasterizationStateCreateInfo()
			.setRasterizerDiscardEnable(VK_FALSE)
			.setPolygonMode(vk::PolygonMode::eFill)
			.setLineWidth(1.0f)
			.setFrontFace(vk::FrontFace::eCounterClockwise)
			.setDepthClampEnable(VK_FALSE)
			.setDepthBiasEnable(VK_FALSE)
			.setDepthBiasSlopeFactor(0.0f)
			.setDepthBiasConstantFactor(0.0f)
			.setDepthBiasClamp(0.0f)
			.setCullMode(vk::CullModeFlagBits::eBack);

		// multi-sample state
		vk::PipelineMultisampleStateCreateInfo multisampleInfo = vk::PipelineMultisampleStateCreateInfo()
			.setSampleShadingEnable(VK_FALSE)
			.setRasterizationSamples(vk::SampleCountFlagBits::e1)
			.setMinSampleShading(1.0f)
			.setAlphaToOneEnable(VK_FALSE)
			.setAlphaToCoverageEnable(VK_FALSE);

		// color blending
		vk::PipelineColorBlendAttachmentState blendAttachment = vk::PipelineColorBlendAttachmentState()
			.setBlendEnable(VK_FALSE)
			.setSrcColorBlendFactor(vk::BlendFactor::eOne)
			.setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
			.setDstColorBlendFactor(vk::BlendFactor::eOne)
			.setDstAlphaBlendFactor(vk::BlendFactor::eOne)
			.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
			.setColorBlendOp(vk::BlendOp::eAdd)
			.setAlphaBlendOp(vk::BlendOp::eAdd);

		vk::PipelineColorBlendStateCreateInfo colorBlendInfo = vk::PipelineColorBlendStateCreateInfo()
			.setAttachmentCount(1)
			.setPAttachments(&blendAttachment)
			.setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f })
			.setLogicOpEnable(VK_FALSE)
			.setLogicOp(vk::LogicOp::eCopy);

		// depth stencil state
		vk::PipelineDepthStencilStateCreateInfo depthStencil = vk::PipelineDepthStencilStateCreateInfo()
			.setBack({})
			.setDepthBoundsTestEnable(VK_FALSE)
			.setDepthCompareOp(vk::CompareOp::eLess)
			.setDepthTestEnable(VK_TRUE)
			.setDepthWriteEnable(VK_TRUE)
			.setFront({})
			.setMaxDepthBounds(1.0f)
			.setMinDepthBounds(0.0f)
			.setStencilTestEnable(VK_FALSE);

		// TODO read about pipeline cache
		//pipeline
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo = vk::PipelineLayoutCreateInfo()
			.setSetLayoutCount(1)
			.setPSetLayouts(&resourceManager.getDescriptor().mLayout)
			.setPushConstantRangeCount(0)
			.setPPushConstantRanges(nullptr);

		MRK_CATCH(layout_ = g_graphicsSystemSingleton.device.logicalDevice_.createPipelineLayout(pipelineLayoutInfo));

		vk::GraphicsPipelineCreateInfo pipelineInfo = vk::GraphicsPipelineCreateInfo()
			.setBasePipelineHandle(vk::Pipeline())
			.setBasePipelineIndex(-1)
			.setLayout(layout_)
			.setPColorBlendState(&colorBlendInfo)
			.setPDepthStencilState(&depthStencil)
			.setPDynamicState(dynamic_ ? &dynamicStateInfo : nullptr)
			.setPInputAssemblyState(&inputAssembly)
			.setPMultisampleState(&multisampleInfo)
			.setPRasterizationState(&rasterInfo)
			.setStageCount(static_cast<uint32_t>(shaderStages.size()))
			.setPStages(shaderStages.data())
			.setPTessellationState(nullptr)
			.setPVertexInputState(&vertexInputInfo)
			.setPViewportState(&viewportStateInfo)
			.setRenderPass(g_graphicsSystemSingleton.swapChain.getRenderPass())
			.setSubpass(0);

		MRK_CATCH(pipeline_ = g_graphicsSystemSingleton.device.logicalDevice_.createGraphicsPipelines(vk::PipelineCache(), pipelineInfo)[0]);

        createCommandBuffers();
    }

	void Pipeline::recreate()
	{
		cleanUp();

        if (dynamic_ == false)
        {
            load(type_);
        }
        else
        {
            createSemaphores();
            createCommandBuffers();
        }
	}

	void Pipeline::cleanUp()
	{
		const vk::Device& dev = g_graphicsSystemSingleton.device.logicalDevice_;

		dev.freeCommandBuffers(g_graphicsSystemSingleton.graphicsPool, commandBuffers_);
        if (dynamic_ == false)
        {
            dev.destroyPipeline(pipeline_);
            dev.destroyPipelineLayout(layout_);
        }

		dev.destroySemaphore(imageAvailable);
		dev.destroySemaphore(renderFinished);
	}

    Pipeline::~Pipeline()
    {
		const vk::Device& dev = g_graphicsSystemSingleton.device.logicalDevice_;

        if (dynamic_ == true)
        {
            dev.destroyPipeline(pipeline_);
            dev.destroyPipelineLayout(layout_);
        }
    }

    void Pipeline::createCommandBuffers()
	{
		commandBuffers_.resize(g_graphicsSystemSingleton.swapChain.getFrameBufferSize());

		vk::CommandBufferAllocateInfo allocInfo = vk::CommandBufferAllocateInfo()
			.setCommandBufferCount(static_cast<uint32_t>(commandBuffers_.size()))
			.setCommandPool(g_graphicsSystemSingleton.graphicsPool)
			.setLevel(vk::CommandBufferLevel::ePrimary);

		MRK_CATCH(commandBuffers_ = g_graphicsSystemSingleton.device.logicalDevice_.allocateCommandBuffers(allocInfo));

		const ResourceManager & resourceManager = g_graphicsSystemSingleton.resourceManager;

		// all of 'this' is used in the loop below
		std::array<vk::ClearValue, 2> clearValues = {};
		clearValues[0].color = vk::ClearColorValue(std::array<float, 4> { 0.0f, 0.0f, 0.0f, 1.0f });
		clearValues[1].depthStencil = vk::ClearDepthStencilValue({ 1.0f, 0 });

		std::array<vk::Buffer, 1> vertexBuffers = {resourceManager.getVertexBuffer().buffer_};
		std::array<vk::DeviceSize, 1> offsets = { 0 };

		const mrk::Buffer & indexBuffer = resourceManager.getIndexBuffer();
		std::array<vk::Buffer, 1> indexBuffers = { indexBuffer.buffer_ };


        auto beginInfo = vk::CommandBufferBeginInfo()
            .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse)
            .setPInheritanceInfo(nullptr);// only for secondary command buffers

		int i = 0; // end of 'this'
		for (const auto& buffer : commandBuffers_)
		{
			buffer.begin(beginInfo);

			vk::RenderPassBeginInfo renderPassInfo = g_graphicsSystemSingleton.swapChain.getRenderPassBeginInfo(i++); // note the i++
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

            if (dynamic_)
            {
		        vk::Rect2D scissor = vk::Rect2D()
		        	.setOffset({ 0,0 })
		        	.setExtent(g_graphicsSystemSingleton.swapChain.getExtent());
		        vk::Viewport viewport = vk::Viewport()
		        	.setX(0.0f)
		        	.setY(0.0f)
		        	.setWidth(static_cast<float>(scissor.extent.width))
		        	.setHeight(static_cast<float>(scissor.extent.height))
		        	.setMinDepth(0.0f)
		        	.setMaxDepth(1.0f);

                buffer.setViewport(0, 1, &viewport);
                buffer.setScissor(0, 1, &scissor);
            }

			buffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

                buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_);

                buffer.bindVertexBuffers(0, vertexBuffers, offsets);
                buffer.bindIndexBuffer(indexBuffers[0], 0, vk::IndexType::eUint32);

                buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout_, 0, resourceManager.getDescriptor().mSet, {/* this should be 0 */});

			    buffer.drawIndexed(static_cast<uint32_t>(indexBuffer.mSize) / sizeof(uint32_t), 1, 0, 0, 0);

			buffer.endRenderPass();

			MRK_CATCH(buffer.end());
		}
	}

	void Pipeline::createSemaphores()
	{
		vk::SemaphoreCreateInfo createInfo = {};

		MRK_CATCH(imageAvailable = g_graphicsSystemSingleton.device.logicalDevice_.createSemaphore(createInfo));
		MRK_CATCH(renderFinished = g_graphicsSystemSingleton.device.logicalDevice_.createSemaphore(createInfo));
	}
}
