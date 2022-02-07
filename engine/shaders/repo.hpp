//@	{
//@	 "dependencies_extra":[{"ref":"./repo.o", "rel":"implementation"}]
//@	}

#include <span>
#include <cstdint>

namespace idis::shaders::repo
{
	std::span<uint32_t const> get_vertex_shader();

	std::span<uint32_t const> get_fragment_shader();
}