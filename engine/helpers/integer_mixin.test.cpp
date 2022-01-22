//@	{"target":{"name":"integer_mixin.test"}}

#include "./integer_mixin.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

namespace
{
	class my_special_integer:public idis::integer_mixin<int, my_special_integer>
	{
	public:
		using base = idis::integer_mixin<int, my_special_integer>;
		using base::base;
	};

	std::string to_string(my_special_integer val)
	{
		return std::to_string(val.value());
	}
}

TESTCASE(integer_mixin_value)
{
	my_special_integer n{1};
	EXPECT_EQ(n.value(), 1);
}

TESTCASE(integer_mixin_comparison)
{
	my_special_integer a{1};
	my_special_integer b{2};
	my_special_integer c{2};
	my_special_integer d{3};

	EXPECT_LT(a, b);
	EXPECT_LT(c, d);

	EXPECT_GT(b, a);
	EXPECT_GT(d, c);

	EXPECT_EQ(b, c);
	EXPECT_NE(a, b);
}

TESTCASE(integer_mixin_preincrement)
{
	my_special_integer a{1};
	EXPECT_EQ(++a, (my_special_integer{2}));
	EXPECT_EQ(a, (my_special_integer{2}));
}

TESTCASE(integer_mixin_postincrement)
{
	my_special_integer a{1};
	EXPECT_EQ(a++, (my_special_integer{1}));
	EXPECT_EQ(a, (my_special_integer{2}));
}

TESTCASE(integer_mixin_predecrement)
{
	my_special_integer a{1};
	EXPECT_EQ(--a, (my_special_integer{0}));
	EXPECT_EQ(a, (my_special_integer{0}));
}

TESTCASE(integer_mixin_postdecrement)
{
	my_special_integer a{1};
	EXPECT_EQ(a--, (my_special_integer{1}));
	EXPECT_EQ(a, (my_special_integer{0}));
}
