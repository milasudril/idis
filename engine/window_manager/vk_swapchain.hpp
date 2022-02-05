//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_swapchain.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_VKSWAPCHAIN_HPP
#define IDIS_WINDOWMANAGER_VKSWAPCHAIN_HPP

#include "./vk_device.hpp"
#include "./vk_surface.hpp"

//#include <vulkan/vulkan.h>

#include <type_traits>
#include <memory>

namespace idis::wm
{
	namespace detail
	{
		class vk_swapchain_deleter
		{
		public:
			explicit vk_swapchain_deleter(VkDevice device): m_device{device} {}

			void operator()(VkSwapchainKHR obj) const
			{
				if(obj != nullptr) { vkDestroySwapchainKHR(m_device, obj, nullptr); }
			}

		private:
			VkDevice m_device;
		};
	}

	class vk_swapchain
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkSwapchainKHR>, detail::vk_swapchain_deleter>;

		vk_swapchain() = default;

		explicit vk_swapchain(vk_device& device,
		                      vk_surface& surface,
		                      uint32_t img_count,
		                      VkSurfaceFormatKHR const& surface_format,
		                      VkExtent2D const& image_extent,
		                      VkPresentModeKHR present_mode,
		                      VkSurfaceTransformFlagBitsKHR transform);

		VkSwapchainKHR handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif