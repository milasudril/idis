//@	{
//@	 "target":{"name":"pipeline.o"}
//@	}

#include "./pipeline.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"

namespace
{
	auto create_pipeline(VkDevice device,
	                     idis::gpu_res::pipeline_descriptor const& descriptor,
	                     VkRenderPass matching_rp)
	{
		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		auto const& shader_program = descriptor.shader_program();
		constexpr auto stage_count = std::tuple_size_v<decltype(shader_program.modules)>;
		std::array<VkPipelineShaderStageCreateInfo, stage_count> stages{
		    get_shader_stage_info(std::get<0>(shader_program.modules)),
		    get_shader_stage_info(std::get<1>(shader_program.modules))};
		pipeline_info.stageCount          = stage_count;
		pipeline_info.pStages             = std::data(stages);
		pipeline_info.layout              = shader_program.layout.handle();
		pipeline_info.pVertexInputState   = &descriptor.vertex_input();
		pipeline_info.pInputAssemblyState = &descriptor.input_assembly();
		pipeline_info.pViewportState      = &descriptor.viewport_state();
		pipeline_info.pRasterizationState = &descriptor.rasterization_state();
		pipeline_info.pMultisampleState   = &descriptor.multisample_state();
		pipeline_info.pColorBlendState    = &descriptor.color_blend_state();
		pipeline_info.renderPass          = matching_rp;
		pipeline_info.subpass             = 0;
		pipeline_info.basePipelineHandle  = VK_NULL_HANDLE;

		VkPipeline ret{};
		if(auto res =
		       vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &ret);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create graphics pipeline", to_string(idis::vk_init::error{res})};
		}

		return idis::gpu_res::pipeline::handle_type{ret, idis::gpu_res::pipeline_deleter{device}};
	}
}

idis::gpu_res::pipeline::pipeline(VkDevice device,
                                  pipeline_descriptor const& descriptor,
                                  VkRenderPass matching_rp)
    : m_handle{create_pipeline(device, descriptor, matching_rp)}
{
}
