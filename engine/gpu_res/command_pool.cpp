//@	{
//@	 "target":{"name":"command_pool.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./command_pool.hpp"

namespace
{
	auto create_command_pool(idis::vk_init::device& device)
	{
		VkCommandPoolCreateInfo create_info{};
		create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		create_info.queueFamilyIndex = device.graphics_queue_family();

		VkCommandPool ret{};

		if(vkCreateCommandPool(device.handle(), &create_info, nullptr, &ret) != VK_SUCCESS)
		{
			throw idis::exception{"create command buffer", ""};
		}

		return idis::gpu_res::command_pool::handle_type{
		    ret, idis::gpu_res::command_pool_deleter{device.handle()}};
	}
}


idis::gpu_res::command_pool::command_pool(vk_init::device& dev): m_handle{create_command_pool(dev)}
{
}
