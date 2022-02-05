//@	{
//@	 "target":{"name":"vk_swapchain.o"}
//@	}

#include "./vk_swapchain.hpp"

namespace
{
	auto create_vk_swapchain(idis::wm::vk_device& device)
	{
		VkSwapchainCreateInfoKHR create_info{};
		VkSwapchainKHR swapchain{};
		if(vkCreateSwapchainKHR(device.handle(), &create_info, nullptr, &swapchain) != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan swapchain", ""};
		}

		return idis::wm::vk_swapchain::handle_type{
		    swapchain, idis::wm::detail::vk_swapchain_deleter{device.handle()}};
	}
}

idis::wm::vk_swapchain::vk_swapchain(vk_device& device): m_handle{create_vk_swapchain(device)} {}