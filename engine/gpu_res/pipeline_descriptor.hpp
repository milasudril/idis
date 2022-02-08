#ifndef IDIS_GPURES_PIPELINEDESCRIPTOR_HPP
#define IDIS_GPURES_PIPELINEDESCRIPTOR_HPP

#include "./shader_module.hpp"

namespace idis::gpu_res
{
	inline auto init_vertex_input()
	{
		VkPipelineVertexInputStateCreateInfo ret;
		ret.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		ret.vertexBindingDescriptionCount   = 0;
		ret.vertexAttributeDescriptionCount = 0;
		return ret;
	}

	inline auto init_input_assembly()
	{
		VkPipelineInputAssemblyStateCreateInfo ret;
		ret.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		ret.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		ret.primitiveRestartEnable = VK_FALSE;
		return ret;
	}

	inline auto init_viewport()
	{
		VkViewport ret{};
		ret.minDepth = 0.0f;
		ret.maxDepth = 1.0f;
		return ret;
	}

	inline auto init_rasterization_state()
	{
		VkPipelineRasterizationStateCreateInfo ret{};
		ret.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		ret.depthClampEnable        = VK_FALSE;
		ret.rasterizerDiscardEnable = VK_FALSE;
		ret.polygonMode             = VK_POLYGON_MODE_FILL;
		ret.lineWidth               = 1.0f;
		ret.cullMode                = VK_CULL_MODE_BACK_BIT;
		ret.frontFace               = VK_FRONT_FACE_CLOCKWISE;
		ret.depthBiasEnable         = VK_FALSE;
		return ret;
	}

	inline auto init_multisample_state()
	{
		VkPipelineMultisampleStateCreateInfo ret{};
		ret.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		ret.sampleShadingEnable  = VK_FALSE;
		ret.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		return ret;
	}

	inline auto init_color_blend_attchment_state()
	{
		VkPipelineColorBlendAttachmentState ret;
		ret.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
		                     | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		ret.blendEnable = VK_FALSE;
		return ret;
	}

	class pipeline_descriptor
	{
	public:
		pipeline_descriptor()
		    : m_vertex_input{init_vertex_input()}
		    , m_input_assembly{init_input_assembly()}
		    , m_viewport{init_viewport()}
		    , m_rasterization_state{init_rasterization_state()}
		    , m_multisample_state{init_multisample_state()}
		    , m_color_blend_attachment_state{init_color_blend_attchment_state()}
		{
		}

		template<auto... Tags>
		using shader_program = std::tuple<shader_module<Tags>...>;

		template<auto ShaderStage>
		pipeline_descriptor& shader(shader_module<ShaderStage>&& shader_mod)
		{
			std::get<shader_module<ShaderStage>>(m_shaders) = std::move(shader_mod);
			return *this;
		}

		pipeline_descriptor& viewport(wm::dimensions dim)
		{
			m_viewport.width  = static_cast<float>(dim.width);
			m_viewport.height = static_cast<float>(dim.height);
			return *this;
		}

		pipeline_descriptor& scissor(wm::dimensions dim)
		{
			m_scissor.extent =
			    VkExtent2D{static_cast<uint32_t>(dim.width), static_cast<uint32_t>(dim.height)};
			return *this;
		}

	private:
		shader_program<VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT> m_shaders;
		VkPipelineVertexInputStateCreateInfo m_vertex_input;
		VkPipelineInputAssemblyStateCreateInfo m_input_assembly;
		VkViewport m_viewport;
		VkRect2D m_scissor{};
		VkPipelineRasterizationStateCreateInfo m_rasterization_state;
		VkPipelineMultisampleStateCreateInfo m_multisample_state;
		VkPipelineColorBlendAttachmentState m_color_blend_attachment_state;
	};
}

#endif