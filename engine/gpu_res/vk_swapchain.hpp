//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_swapchain.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKSWAPCHAIN_HPP
#define IDIS_GPURES_VKSWAPCHAIN_HPP

#include "engine/vk_init/device.hpp"
#include "engine/vk_init/surface.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class vk_swapchain_deleter
	{
	public:
		explicit vk_swapchain_deleter(VkDevice device): m_device{device} {}

		void operator()(VkSwapchainKHR obj) const
		{
			if(obj != nullptr) { vkDestroySwapchainKHR(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class vk_swapchain
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkSwapchainKHR>, vk_swapchain_deleter>;

		vk_swapchain() = default;

		explicit vk_swapchain(vk_init::device& device, vk_init::surface& surface);

		VkSwapchainKHR handle() const { return m_handle.get(); }

		std::vector<VkImage> get_images() const
		{
			uint32_t img_count{};
			vkGetSwapchainImagesKHR(
			    m_handle.get_deleter().device(), m_handle.get(), &img_count, nullptr);
			std::vector<VkImage> ret(img_count);
			vkGetSwapchainImagesKHR(
			    m_handle.get_deleter().device(), m_handle.get(), &img_count, std::data(ret));
			return ret;
		}

	private:
		handle_type m_handle;
	};
}

#endif