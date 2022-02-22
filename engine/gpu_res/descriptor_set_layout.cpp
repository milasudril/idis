//@	{"target":{"name":"descriptor_set_layout.o"}}

#include "./descriptor_set_layout.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"

idis::gpu_res::descriptor_set_layout_handle idis::gpu_res::create_descriptor_set_layout(
    VkDevice device,
    VkDescriptorSetLayoutCreateFlags flags,
    std::span<VkDescriptorSetLayoutBinding const> bindings)
{
	VkDescriptorSetLayoutCreateInfo create_info{};
	create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	create_info.flags        = flags;
	create_info.bindingCount = static_cast<uint32_t>(std::size(bindings));
	create_info.pBindings    = std::data(bindings);
	VkDescriptorSetLayout ret{};
	if(auto res = vkCreateDescriptorSetLayout(device, &create_info, nullptr, &ret);
	   res != VK_SUCCESS)
	{
		throw idis::exception{"create descriptor binding set layout",
		                      to_string(idis::vk_init::error{res})};
	}

	return idis::gpu_res::descriptor_set_layout_handle{
	    ret, idis::gpu_res::descriptor_set_layout_deleter{device}};
}