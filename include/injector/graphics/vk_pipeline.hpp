#pragma once
#include <injector/graphics/pipeline.hpp>
#include <vulkan/vulkan.hpp>

namespace INJECTOR_NAMESPACE
{
	class VkPipeline : public Pipeline
	{
	protected:
		vk::Device device;
		vk::PipelineCache pipelineCache;
	public:
		VkPipeline(vk::Device device);
		virtual ~VkPipeline();

		virtual void bind(
			vk::CommandBuffer commandBuffer);
		virtual void recreate(
			vk::RenderPass renderPass,
			vk::Extent2D surfaceExtent);
	};

	using VkPipelineHandle = std::shared_ptr<VkPipeline>;
}
