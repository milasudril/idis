//@	{
//@		"target":{"name":"repo.o"}
//@		,"dependencies":[
//@			{"ref":"./my_vertex_shader.spv", "origin":"generated"},
//@			{"ref":"./my_fragment_shader.spv", "origin":"generated"}
//@		]
//@	}

#include "engine/helpers/incbin.hpp"

IDIS_INCBIN(vertex_shader, MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/my_vertex_shader.spv");
IDIS_INCBIN(fragment_shader, MAIKE_BUILDINFO_TARGETDIR "/engine/shaders/my_fragment_shader.spv");

namespace idis::shaders::repo
{
}
