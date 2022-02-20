//@{"target":{"name":"testprog.vert.spv"}}

#include "./testprog_def.hpp"

layout(location = 0) out vec4 frag_color;

void main()
{
	gl_Position = vec4(loc, 0.0, 1.0);
	frag_color = vert_color;
}
