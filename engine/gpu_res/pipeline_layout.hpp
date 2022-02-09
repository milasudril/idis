//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./pipeline_layout.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_PIPELINELAYOUT_HPP
#define IDIS_GPURES_PIPELINELAYOUT_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class pipeline_layout_deleter
	{
	public:
		explicit pipeline_layout_deleter(VkDevice device): m_device{device} {}

		void operator()(VkPipelineLayout obj) const
		{
			if(obj != nullptr) { vkDestroyPipelineLayout(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class pipeline_layout
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkPipelineLayout>, pipeline_layout_deleter>;
		;

		pipeline_layout(): m_handle{nullptr, pipeline_layout_deleter{nullptr}} {}

		explicit pipeline_layout(init::device& device): pipeline_layout{device.handle()} {}

		explicit pipeline_layout(VkDevice device);

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif