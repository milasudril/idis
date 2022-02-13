//@	{"target":{"name":"pipeline_layout.o"}}

#include "./pipeline_layout.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"

namespace
{
	auto create_pipeline_layout(VkDevice device)
	{
		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount         = 0;        // Optional
		pipeline_layout_info.pSetLayouts            = nullptr;  // Optional
		pipeline_layout_info.pushConstantRangeCount = 0;        // Optional
		pipeline_layout_info.pPushConstantRanges    = nullptr;  // Optional

		VkPipelineLayout ret{};
		if(auto res = vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &ret);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create pipeline layout", to_string(idis::vk_init::error{res})};
		}

		return idis::gpu_res::pipeline_layout::handle_type{
		    ret, idis::gpu_res::pipeline_layout_deleter{device}};
	}
}


idis::gpu_res::pipeline_layout::pipeline_layout(VkDevice device)
    : m_handle{create_pipeline_layout(device)}
{
}