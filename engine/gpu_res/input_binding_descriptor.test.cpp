//@	{"target":{"name":"input_binding_descriptor.test"}}

#include "./input_binding_descriptor.hpp"

#include "testfwk/testfwk.hpp"

#include <tuple>

namespace
{
	struct testprog
	{
		using input_port_types = std::tuple<idis::vec2f_t, idis::vec4f_t>;
	};
}

TESTCASE(idis_gpures_multi_input_binding_descriptor)
{
	idis::gpu_res::input_binding_descriptor<testprog> obj;
	EXPECT_EQ(obj.num_inputs, 2);

	auto const bindings = obj.bindings;
	EXPECT_EQ(std::size(bindings), 2);
	EXPECT_EQ(bindings[0].binding, 0);
	EXPECT_EQ(bindings[0].stride, sizeof(idis::vec2f_t));
	EXPECT_EQ(bindings[0].inputRate, VK_VERTEX_INPUT_RATE_VERTEX);
	EXPECT_EQ(bindings[1].binding, 1);
	EXPECT_EQ(bindings[1].stride, sizeof(idis::vec4f_t));
	EXPECT_EQ(bindings[1].inputRate, VK_VERTEX_INPUT_RATE_VERTEX);

	auto const attribs = obj.attributes;
	EXPECT_EQ(std::size(attribs), 2);
	EXPECT_EQ(attribs[0].binding, 0);
	EXPECT_EQ(attribs[0].location, 0);
	EXPECT_EQ(attribs[0].format, VK_FORMAT_R32G32_SFLOAT);
	EXPECT_EQ(attribs[0].offset, 0);
	EXPECT_EQ(attribs[1].location, 1);
	EXPECT_EQ(attribs[1].binding, 1);
	EXPECT_EQ(attribs[1].format, VK_FORMAT_R32G32B32A32_SFLOAT);
	EXPECT_EQ(attribs[1].offset, 0);
}