//@	{
//@	 "target":{"name":"pipeline.o"}
//@	}

#include "./pipeline.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"


namespace
{
	auto get_vertex_input_state(
	    std::reference_wrapper<idis::gpu_res::shader_program_info const> prg)
	{
		VkPipelineVertexInputStateCreateInfo ret{};
		ret.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		ret.vertexBindingDescriptionCount   = std::size(prg.get().input_bindings);
		ret.pVertexBindingDescriptions      = std::data(prg.get().input_bindings);
		ret.vertexAttributeDescriptionCount = std::size(prg.get().input_attributes);
		ret.pVertexAttributeDescriptions    = std::data(prg.get().input_attributes);
		return ret;
	}
}

idis::gpu_res::pipeline_handle_type idis::gpu_res::create_pipeline(
    VkDevice device,
    pipeline_descriptor const& descriptor,
    shader_program_info const& shader_program,
    VkRenderPass matching_rp)
{
	VkGraphicsPipelineCreateInfo pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	constexpr auto stage_count = std::tuple_size_v<decltype(shader_program.modules)>;
	static_assert(stage_count == 2);
	std::array<VkPipelineShaderStageCreateInfo, stage_count> stages{
	    get_shader_stage_info(std::get<0>(shader_program.modules)),
	    get_shader_stage_info(std::get<1>(shader_program.modules))};
	pipeline_info.stageCount          = stage_count;
	pipeline_info.pStages             = std::data(stages);
	pipeline_info.layout              = shader_program.layout.handle();
	auto const vertex_input_state     = get_vertex_input_state(shader_program);
	pipeline_info.pVertexInputState   = &vertex_input_state;
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

	return idis::gpu_res::pipeline_handle_type{ret, idis::gpu_res::pipeline_deleter{device}};
}