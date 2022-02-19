//@	{"target":{"name":"vec_t.test"}}

#include "./vec_t.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_vect_dot)
{
	idis::vec4i_t a{1, 2, 3, 4};
	idis::vec4i_t b{2, 3, 4, 5};

	auto const res = idis::dot(a, b);
	EXPECT_EQ(res, 2 + 6 + 12 + 20);
}

TESTCASE(idis_vect_length_squared)
{
	auto const res = idis::length_squared(idis::vec4i_t{1, 2, 3, 4});
	EXPECT_EQ(res, 1 + 4 + 9 + 16);
}

TESTCASE(idis_vect_length)
{
	auto const res = idis::length(idis::vec2f_t{3.0f, 4.0f});
	EXPECT_EQ(res, 5.0f);
}

TESTCASE(idis_vect_normalized)
{
	auto const res = idis::normalized(idis::vec2f_t{3.0f, 4.0f});
	EXPECT_EQ(res[0], 3.0f / 5.0f);
	EXPECT_EQ(res[1], 4.0f / 5.0f);
}

TESTCASE(idis_vect_vector_cast_4)
{
	auto const res = idis::vector_cast<idis::vec4i_t>(idis::vec4f_t{1.5f, 2.25f, 2.7f, 2.5f});
	EXPECT_EQ(res[0], 1);
	EXPECT_EQ(res[1], 2);
	EXPECT_EQ(res[2], 2);
	EXPECT_EQ(res[3], 2);
}

TESTCASE(idis_vect_vector_cast_2)
{
	auto const res = idis::vector_cast<idis::vec2i_t>(idis::vec2f_t{1.5f, 2.25f});
	EXPECT_EQ(res[0], 1);
	EXPECT_EQ(res[1], 2);
}