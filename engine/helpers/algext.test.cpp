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

TESTCASE(idis_algext_for_each)
{
	std::array<int, 3> foo{0, 2, 4};
	std::array<int, 3> bar{1, 3, 5};

	idis::for_each(
	    [k = 0](auto a, auto b) mutable
	    {
		    static_assert(std::is_same_v<decltype(a), int>);
		    static_assert(std::is_same_v<decltype(b), int>);
		    EXPECT_EQ(a, 2 * k);
		    EXPECT_EQ(b, 2 * k + 1);
		    ++k;
	    },
	    foo,
	    bar);
}