//@	{
//@		"target":{"name":"repo.o"}
//@		,"dependencies":[
//@			{"ref":"./my_vertex_shader.vert.spv", "origin":"generated"},
//@			{"ref":"./my_fragment_shader.frag.spv", "origin":"generated"}
//@		]
//@	}

#include "./repo.hpp"
#include "engine/helpers/incbin.hpp"

IDIS_INCBIN_U32(vertex_shader, MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/my_vertex_shader.vert.spv");
IDIS_INCBIN_U32(fragment_shader,
                MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/my_fragment_shader.frag.spv");

std::span<uint32_t const> idis::shaders::repo::get_vertex_shader()
{
	return std::span{vertex_shader_begin, vertex_shader_end};
}

std::span<uint32_t const> idis::shaders::repo::get_fragment_shader()
{
	return std::span{fragment_shader_begin, fragment_shader_end};
}
