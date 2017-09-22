#pragma once
#include <vulkan/vulkan.hpp>

namespace mrk
{

    class Pipeline
    {
    public:
		vk::PipelineLayout layout_;
		vk::Pipeline pipeline_;
		std::vector<vk::CommandBuffer> commandBuffers_;

		vk::Semaphore imageAvailable;
		vk::Semaphore renderFinished;

		Pipeline();
		~Pipeline();
        void load();
		void recreate();
		
    private:
		void cleanUp();
		void createCommandBuffers();
		void createSemaphores();
	};
}
