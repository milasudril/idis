//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./render_pass.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_RENDERPASS_HPP
#define IDIS_GPURES_RENDERPASS_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class render_pass_deleter
	{
	public:
		explicit render_pass_deleter(VkDevice device): m_device{device} {}

		void operator()(VkRenderPass obj) const
		{
			if(obj != nullptr) { vkDestroyRenderPass(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class render_pass
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkRenderPass>, render_pass_deleter>;
		;

		render_pass(): m_handle{nullptr, render_pass_deleter{nullptr}} {}


		explicit render_pass(vk_init::device& device, VkFormat output_format)
		    : render_pass{device.handle(), output_format}
		{
		}


		explicit render_pass(VkDevice device, VkFormat output_format);

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif