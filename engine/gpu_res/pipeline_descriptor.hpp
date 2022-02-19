#ifndef IDIS_GPURES_PIPELINEDESCRIPTOR_HPP
#define IDIS_GPURES_PIPELINEDESCRIPTOR_HPP

#include "./shader_module.hpp"
#include "./pipeline_layout.hpp"

namespace idis::gpu_res
{
	inline auto init_vertex_input()
	{
		VkPipelineVertexInputStateCreateInfo ret{};
		ret.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		ret.vertexBindingDescriptionCount   = 0;
		ret.vertexAttributeDescriptionCount = 0;
		return ret;
	}

	inline auto init_input_assembly()
	{
		VkPipelineInputAssemblyStateCreateInfo ret{};
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

	inline auto init_viewport_state(std::reference_wrapper<VkViewport const> viewport,
	                                std::reference_wrapper<VkRect2D const> scissor)
	{
		VkPipelineViewportStateCreateInfo ret{};
		ret.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		ret.viewportCount = 1;
		ret.pViewports    = &viewport.get();
		ret.scissorCount  = 1;
		ret.pScissors     = &scissor.get();
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
		VkPipelineColorBlendAttachmentState ret{};
		ret.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
		                     | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		ret.blendEnable = VK_FALSE;
		return ret;
	}

	inline auto init_color_blend_state(
	    std::reference_wrapper<VkPipelineColorBlendAttachmentState const> attachment)
	{
		VkPipelineColorBlendStateCreateInfo ret{};
		ret.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		ret.logicOpEnable     = VK_FALSE;
		ret.logicOp           = VK_LOGIC_OP_COPY;
		ret.attachmentCount   = 1;
		ret.pAttachments      = &attachment.get();
		ret.blendConstants[0] = 0.0f;
		ret.blendConstants[1] = 0.0f;
		ret.blendConstants[2] = 0.0f;
		ret.blendConstants[3] = 0.0f;
		return ret;
	}

	template<auto... Tags>
	using shader_module_set = std::tuple<shader_module<Tags>...>;

	struct shader_program_info
	{
		shader_module_set<VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT> modules;
		pipeline_layout layout;
	};

	template<class ShaderDescriptor>
	shader_program_info make_shader_program_info(pipeline_layout&& layout)
	{
		return shader_program_info{
		    {shader_module{layout.device(), ShaderDescriptor::vertex_shader()},
		     shader_module{layout.device(), ShaderDescriptor::fragment_shader()}},
		    std::move(layout)};
	}

	class pipeline_descriptor
	{
	public:
		pipeline_descriptor()
		    : m_vertex_input{init_vertex_input()}
		    , m_input_assembly{init_input_assembly()}
		    , m_viewport{std::make_unique<VkViewport>(init_viewport())}
		    , m_scissor{std::make_unique<VkRect2D>()}
		    , m_viewport_state{init_viewport_state(*m_viewport, *m_scissor)}
		    , m_rasterization_state{init_rasterization_state()}
		    , m_multisample_state{init_multisample_state()}
		    , m_color_blend_attachment_state{std::make_unique<VkPipelineColorBlendAttachmentState>(
		          init_color_blend_attchment_state())}
		    , m_color_blend_state{init_color_blend_state(*m_color_blend_attachment_state)}
		    , m_shader_program{nullptr}
		{
		}

		pipeline_descriptor& shader_program(std::reference_wrapper<shader_program_info const> prg)
		{
			m_shader_program = &prg.get();
			return *this;
		}

		pipeline_descriptor& viewport(VkExtent2D dim)
		{
			m_viewport->width  = static_cast<float>(dim.width);
			m_viewport->height = static_cast<float>(dim.height);
			return *this;
		}

		pipeline_descriptor& scissor(VkExtent2D dim)
		{
			m_scissor->extent =
			    VkExtent2D{static_cast<uint32_t>(dim.width), static_cast<uint32_t>(dim.height)};
			return *this;
		}

		auto const& shader_program() const { return *m_shader_program; }

		auto const& vertex_input() const { return m_vertex_input; }

		auto const& input_assembly() const { return m_input_assembly; }

		auto const& viewport_state() const { return m_viewport_state; }

		auto const& rasterization_state() const { return m_rasterization_state; }

		auto const& multisample_state() const { return m_multisample_state; }

		auto const& color_blend_state() const { return m_color_blend_state; }

	private:
		VkPipelineVertexInputStateCreateInfo m_vertex_input;
		VkPipelineInputAssemblyStateCreateInfo m_input_assembly;
		std::unique_ptr<VkViewport> m_viewport;
		std::unique_ptr<VkRect2D> m_scissor;
		VkPipelineViewportStateCreateInfo m_viewport_state;
		VkPipelineRasterizationStateCreateInfo m_rasterization_state;
		VkPipelineMultisampleStateCreateInfo m_multisample_state;
		std::unique_ptr<VkPipelineColorBlendAttachmentState> m_color_blend_attachment_state;
		VkPipelineColorBlendStateCreateInfo m_color_blend_state;
		shader_program_info const* m_shader_program;
	};
}

#endif