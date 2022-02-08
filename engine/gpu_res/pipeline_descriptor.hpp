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

	class pipeline_descriptor
	{
	public:
		pipeline_descriptor()
		    : m_vertex_input{init_vertex_input()}
		    , m_input_assembly{init_input_assembly()}
		    , m_viewport{init_viewport()}
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

	private:
		shader_program<VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT> m_shaders;
		VkPipelineVertexInputStateCreateInfo m_vertex_input;
		VkPipelineInputAssemblyStateCreateInfo m_input_assembly;
		VkViewport m_viewport;
	};
}

#endif