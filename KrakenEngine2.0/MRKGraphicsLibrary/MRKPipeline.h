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

		Pipeline(bool dynamic = true);
        void load();
		void recreate();
		void cleanUp();
        ~Pipeline();

    private:
        const bool dynamic_;

		void createCommandBuffers();
		void createSemaphores();
	};
}
