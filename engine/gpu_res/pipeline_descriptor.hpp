#ifndef IDIS_GPURES_PIPELINEDESCRIPTOR_HPP
#define IDIS_GPURES_PIPELINEDESCRIPTOR_HPP

#include "./shader_module.hpp"

namespace idis::gpu_res
{
	class pipeline_descriptor
	{
	public:
		template<auto ... Tags>
		using shader_program = std::tuple<shader_module<Tags>...>;

		template<auto ShaderStage>
		pipeline_descriptor& shader(shader_module<ShaderStage>&& shader_mod)
		{
			std::get<shader_module<ShaderStage>>(m_shaders) = std::move(shader_mod);
			return *this;
		}

	private:
		shader_program<VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT> m_shaders;
	};
}

#endif