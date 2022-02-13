//@	{
//@	 "target":{"name":"command_pool.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./command_pool.hpp"

namespace
{
	auto create_command_pool(std::reference_wrapper<idis::vk_init::device const> device)
	{
		VkCommandPoolCreateInfo create_info{};
		create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		create_info.queueFamilyIndex = device.get().graphics_queue_family();

		VkCommandPool ret{};

		if(vkCreateCommandPool(device.get().handle(), &create_info, nullptr, &ret) != VK_SUCCESS)
		{
			throw idis::exception{"create command buffer", ""};
		}

		return idis::gpu_res::command_pool::handle_type{
		    ret, idis::gpu_res::command_pool_deleter{device.get().handle()}};
	}
}


idis::gpu_res::command_pool::command_pool(std::reference_wrapper<idis::vk_init::device const> dev)
    : m_handle{create_command_pool(dev)}
{
}

idis::gpu_res::command_buffer_set::command_buffer_set(command_pool& pool, size_t n)
    : m_storage(n)
    , m_owner{&pool}
{
	VkCommandBufferAllocateInfo create_info{};
	create_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	create_info.commandPool        = pool.handle();
	create_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	create_info.commandBufferCount = static_cast<size_t>(n);

	if(vkAllocateCommandBuffers(pool.device(), &create_info, std::data(m_storage)) != VK_SUCCESS)
	{
		throw exception{"create command buffers ", ""};
	}
}
