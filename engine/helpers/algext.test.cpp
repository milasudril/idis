//@	{"target":{"name":"algext.test"}}

#include "./algext.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_algext_split)
{
	std::string str{"Foo bar kaka "};
	auto str_split = idis::split(str, ' ');
	REQUIRE_EQ(std::size(str_split), 4);
	EXPECT_EQ(str_split[0], "Foo");
	EXPECT_EQ(str_split[1], "bar");
	EXPECT_EQ(str_split[2], "kaka");
	EXPECT_EQ(str_split[3], "");
}
