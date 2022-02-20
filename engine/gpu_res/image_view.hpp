//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./image_view.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKIMAGEVIEW_HPP
#define IDIS_GPURES_VKIMAGEVIEW_HPP

#include "./image.hpp"

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

		image_view():m_handle{nullptr, image_view_deleter{nullptr}}{}

		explicit image_view(std::reference_wrapper<vk_init::device const> device,
		                    VkImage image,
		                    VkFormat format)
		    : image_view{device.get().handle(), image, format}
		{
		}

		template<image_descriptor Descriptor, auto AllocationFlags, auto AllocationRequirements>
		explicit image_view(std::reference_wrapper<image<Descriptor, AllocationFlags, AllocationRequirements> const> img):
		image_view{img.get().device(), img.get().handle(), img.get().descriptor}
		{
		}

		explicit image_view(VkDevice device, VkImage image, VkFormat format);

		explicit image_view(VkDevice device, VkImage image, image_descriptor const& descriptor);

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif