//@	{
//@		"target":{"name":"repo.o"}
//@		,"dependencies":[
//@			{"ref":"./my_vertex_shader.vert.spv", "origin":"generated"},
//@			{"ref":"./my_fragment_shader.frag.spv", "origin":"generated"}
//@		]
//@	}

#include "./repo.hpp"
#include "engine/helpers/incbin.hpp"

IDIS_INCBIN_U32(vertex_shader,
                MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/my_vertex_shader.vert.spv");

idis::shaders::vertex_shader_source<std::span<uint32_t const>> idis::shaders::repo::
    get_vertex_shader()
{
	return vertex_shader_source{std::span{vertex_shader_begin, vertex_shader_end}};
}

IDIS_INCBIN_U32(fragment_shader,
                MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/my_fragment_shader.frag.spv");

idis::shaders::fragment_shader_source<std::span<uint32_t const>> idis::shaders::repo::
    get_fragment_shader()
{
	return fragment_shader_source{std::span{fragment_shader_begin, fragment_shader_end}};
}
