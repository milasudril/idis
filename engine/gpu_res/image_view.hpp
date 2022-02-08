//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./image_view.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKIMAGEVIEW_HPP
#define IDIS_GPURES_VKIMAGEVIEW_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class image_view_deleter
	{
	public:
		explicit image_view_deleter(VkDevice device): m_device{device} {}

		void operator()(VkImageView obj) const
		{
			if(obj != nullptr) { vkDestroyImageView(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class image_view
	{
	public:
		using handle_type = std::unique_ptr<std::remove_pointer_t<VkImageView>, image_view_deleter>;

		image_view() = default;

		explicit image_view(init::device& device, VkImage image, VkFormat format)
		    : image_view{device.handle(), image, format}
		{
		}

		explicit image_view(VkDevice device, VkImage image, VkFormat format);

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif