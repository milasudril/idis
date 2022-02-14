//@	{
//@	 "target":{"name":"command_pool.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./command_pool.hpp"

#include "engine/vk_init/error.hpp"

namespace
{
	auto create_command_pool(std::reference_wrapper<idis::vk_init::device const> device)
	{
		VkCommandPoolCreateInfo create_info{};
		create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		create_info.queueFamilyIndex = device.get().graphics_queue_family();
		create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VkCommandPool ret{};

		if(auto res = vkCreateCommandPool(device.get().handle(), &create_info, nullptr, &ret);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create command pool", to_string(idis::vk_init::error{res})};
		}

		return idis::gpu_res::command_pool::handle_type{
		    ret, idis::gpu_res::command_pool_deleter{device.get().handle()}};
	}
}


idis::gpu_res::command_pool::command_pool(std::reference_wrapper<idis::vk_init::device const> dev)
    : m_handle{create_command_pool(dev)}
{
}

idis::gpu_res::command_buffer_set::command_buffer_set(
    std::reference_wrapper<command_pool const> pool, size_t n)
    : m_storage(n)
    , m_owner{&pool.get()}
{
	VkCommandBufferAllocateInfo create_info{};
	create_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	create_info.commandPool        = m_owner->handle();
	create_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	create_info.commandBufferCount = static_cast<uint32_t>(n);

	if(auto res = vkAllocateCommandBuffers(pool.get().device(), &create_info, std::data(m_storage));
	   res != VK_SUCCESS)
	{
		throw exception{"create command buffers", to_string(idis::vk_init::error{res})};
	}
}
