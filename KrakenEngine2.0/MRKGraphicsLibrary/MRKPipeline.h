#pragma once
#include "Precompiled.h"

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

        explicit Pipeline();
        Pipeline & operator=(Pipeline && other) noexcept;
        void load(ShaderType type);
		void recreate();
		void cleanUp();
        ~Pipeline();

    private:
        const bool dynamic_ = ifProjectIsBuiltInDebugMode();
        ShaderType type_;

		void createCommandBuffers();
		void createSemaphores();
	};
}
