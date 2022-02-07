//@	{"target":{"name":"vk_shader_module.o"}}

#include "./vk_shader_module.hpp"

idis::gpu_res::shader_module_handle idis::gpu_res::create_vk_shader_module(
    VkDevice device, std::span<uint32_t const> spirv_data)
{
	VkShaderModuleCreateInfo create_info{};
	create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = std::size(spirv_data) * sizeof(uint32_t);
	create_info.pCode    = std::data(spirv_data);

	VkShaderModule shader_module{};
	if(vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
	{
		throw exception{"create shader module", ""};
	}

	return shader_module_handle{shader_module, vk_shader_module_deleter{device}};
}