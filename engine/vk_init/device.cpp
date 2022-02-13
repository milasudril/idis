//@	{
//@	 "target":{"name":"./device.o"}
//@	}

#include "./device.hpp"
#include "./error.hpp"

#include "engine/error_handling/exception.hpp"

#include <set>
#include <array>
#include <algorithm>

idis::vk_init::device::device(render_device const& device_info)
    : m_graphics_queue{device_info.graphics_queue_family}
    , m_surface_queue{device_info.surface_queue_family}
    , m_device_info{device_info}
{
	std::set queue_families{m_graphics_queue, m_surface_queue};
	auto const num_queues = std::size(queue_families);
	std::array<VkDeviceQueueCreateInfo, 2> create_infos{};
	std::ranges::for_each(queue_families,
	                      [&create_infos, k = static_cast<size_t>(0)](auto index) mutable
	                      {
		                      VkDeviceQueueCreateInfo info{};
		                      info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		                      info.queueFamilyIndex = index;
		                      auto const queue_prio = 1.0f;
		                      info.queueCount       = 1;
		                      info.pQueuePriorities = &queue_prio;
		                      create_infos[k]       = info;
		                      ++k;
	                      });

	VkPhysicalDeviceFeatures dev_features{};
	VkDeviceCreateInfo create_info{};
	create_info.sType                    = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.queueCreateInfoCount     = static_cast<uint32_t>(num_queues);
	create_info.pQueueCreateInfos        = std::data(create_infos);
	create_info.pEnabledFeatures         = &dev_features;
	create_info.enabledExtensionCount    = 1;
	constexpr char const* swapchain_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	create_info.ppEnabledExtensionNames  = &swapchain_name;

	VkDevice device{};
	if(auto res = vkCreateDevice(device_info.device, &create_info, nullptr, &device); res != VK_SUCCESS)
	{
		throw exception{"create logical device", to_string(error{res})};
	}
	m_handle.reset(device);
}
