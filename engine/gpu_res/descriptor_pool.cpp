//@	{"target":{"name":"descriptor_pool.o"}}

#include "./descriptor_pool.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"

idis::gpu_res::descriptor_pool_handle idis::gpu_res::create_descriptor_pool(
    VkDevice device, uint32_t max_num_sets, std::span<VkDescriptorPoolSize const> pool_sizes)
{
	VkDescriptorPoolCreateInfo create_info{};
	create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	create_info.maxSets       = max_num_sets;
	create_info.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
	create_info.pPoolSizes    = std::data(pool_sizes);
	VkDescriptorPool ret;
	if(auto res = vkCreateDescriptorPool(device, &create_info, nullptr, &ret); res != VK_SUCCESS)
	{
		throw idis::exception{"create render pass", to_string(idis::vk_init::error{res})};
	}

	return idis::gpu_res::descriptor_pool_handle{ret,
	                                             idis::gpu_res::descriptor_pool_deleter{device}};
}