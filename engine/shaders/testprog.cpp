//@	{
//@		"target":{"name":"testprog.o"}
//@		,"dependencies":[
//@			{"ref":"./testprog.vert.spv", "origin":"generated"},
//@			{"ref":"./testprog.frag.spv", "origin":"generated"}
//@		]
//@	}

#include "./testprog_def.hpp"

#include "engine/utils/incbin.hpp"

IDIS_INCBIN_U32(vertex_shader, MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/testprog.vert.spv");

idis::shaders::vertex_shader_source<std::span<uint32_t const>> idis::shaders::testprog::
    vertex_shader()
{
	return vertex_shader_source{std::span{vertex_shader_begin, vertex_shader_end}};
}

IDIS_INCBIN_U32(fragment_shader, MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/testprog.frag.spv");

idis::shaders::fragment_shader_source<std::span<uint32_t const>> idis::shaders::testprog::
    fragment_shader()
{
	return fragment_shader_source{std::span{fragment_shader_begin, fragment_shader_end}};
}
