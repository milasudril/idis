#ifndef IDIS_SHADERS_TESTPROGDEF_HPP
#define IDIS_SHADERS_TESTPROGDEF_HPP

#include "./shader_descriptor.hpp"

IDIS_BEGIN_SHADER_DESCRIPTOR(idis::shaders, testprog)
IDIS_SHADER_INPUT(vec2, loc)
IDIS_SHADER_INPUT(vec4, vert_color)
IDIS_END_SHADER_DESCRIPTOR()

#endif