//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./framebuffer.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_FRAMEBUFFER_HPP
#define IDIS_GPURES_FRAMEBUFFER_HPP

#include "engine/gpu_res/image_view.hpp"
#include "engine/gpu_res/render_pass.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class framebuffer_deleter
	{
	public:
		explicit framebuffer_deleter(VkDevice device): m_device{device} {}

		void operator()(VkFramebuffer obj) const
		{
			if(obj != nullptr) { vkDestroyFramebuffer(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class framebuffer
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkFramebuffer>, framebuffer_deleter>;

		framebuffer() = default;

		explicit framebuffer(render_pass const& matching_rp, VkExtent2D dim, image_view& img_view)
		    : framebuffer{matching_rp.device(), matching_rp.handle(), dim, img_view.handle()}
		{
		}

		explicit framebuffer(VkDevice device,
		                     VkRenderPass matching_rp,
		                     VkExtent2D dim,
		                     VkImageView img_view);

		VkFramebuffer handle() const { return m_handle.get(); }

		auto device() const { return m_handle.get_deleter().device(); }

	private:
		handle_type m_handle;
	};
}

#endif