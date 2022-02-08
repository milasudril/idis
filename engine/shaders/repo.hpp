//@	{
//@	 "dependencies_extra":[{"ref":"./repo.o", "rel":"implementation"}]
//@	}

#include "./shader_source.hpp"

#include <span>

namespace idis::shaders::repo
{
	vertex_shader_source<std::span<uint32_t const>> get_vertex_shader();

	fragment_shader_source<std::span<uint32_t const>> get_fragment_shader();
}