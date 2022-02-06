//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_image_view.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKIMAGEVIEW_HPP
#define IDIS_GPURES_VKIMAGEVIEW_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class vk_image_view_deleter
	{
	public:
		explicit vk_image_view_deleter(VkDevice device): m_device{device} {}

		void operator()(VkImageView obj) const
		{
			if(obj != nullptr) { vkDestroyImageView(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class vk_image_view
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkImageView>, vk_image_view_deleter>;

		vk_image_view() = default;

		explicit vk_image_view(vk_init::device& device, VkImage image, VkFormat format): vk_image_view{device.handle(), image, format}
		{}

		explicit vk_image_view(VkDevice device, VkImage image, VkFormat format);

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif