//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./pipeline.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_PIPELINE_HPP
#define IDIS_GPURES_PIPELINE_HPP

#include "./pipeline_descriptor.hpp"
#include "./render_pass.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class pipeline_deleter
	{
	public:
		explicit pipeline_deleter(VkDevice device): m_device{device} {}

		void operator()(VkPipeline obj) const
		{
			if(obj != nullptr) { vkDestroyPipeline(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class pipeline
	{
	public:
		using handle_type = std::unique_ptr<std::remove_pointer_t<VkPipeline>, pipeline_deleter>;
		;

		pipeline(): m_handle{nullptr, pipeline_deleter{nullptr}} {}


		explicit pipeline(vk_init::device& device,
		                  pipeline_descriptor const& descriptor,
		                  render_pass& rp)
		    : pipeline{device.handle(), descriptor, rp}
		{
		}


		explicit pipeline(VkDevice device, pipeline_descriptor const& descriptor, render_pass& rp);

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif