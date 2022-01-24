//@	{"target":{"name":"integer_mixin.test"}}

#include "./integer_mixin.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

namespace
{
	class my_special_integer: public idis::integer_mixin<int, my_special_integer>
	{
	public:
		using base = idis::integer_mixin<int, my_special_integer>;
		using base::base;
	};

	std::string to_string(my_special_integer val) { return std::to_string(val.value()); }
}

constexpr auto compile_time_val = my_special_integer{1} + my_special_integer{2};
static_assert(compile_time_val == my_special_integer{3});

TESTCASE(idis_integer_mixin_value)
{
	my_special_integer n{1};
	EXPECT_EQ(n.value(), 1);
}

TESTCASE(idis_integer_mixin_zero) { EXPECT_EQ(my_special_integer::zero().value(), 0); }

TESTCASE(idis_integer_mixin_comparison)
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

TESTCASE(idis_integer_mixin_preincrement)
{
	my_special_integer a{1};
	EXPECT_EQ(++a, my_special_integer{2});
	EXPECT_EQ(a, my_special_integer{2});
}

TESTCASE(idis_integer_mixin_postincrement)
{
	my_special_integer a{1};
	EXPECT_EQ(a++, my_special_integer{1});
	EXPECT_EQ(a, my_special_integer{2});
}

TESTCASE(idis_integer_mixin_predecrement)
{
	my_special_integer a{1};
	EXPECT_EQ(--a, my_special_integer{0});
	EXPECT_EQ(a, my_special_integer{0});
}

TESTCASE(idis_integer_mixin_postdecrement)
{
	my_special_integer a{1};
	EXPECT_EQ(a--, my_special_integer{1});
	EXPECT_EQ(a, my_special_integer{0});
}

TESTCASE(idis_integer_mixin_add_assign)
{
	my_special_integer a{1};
	my_special_integer b{2};
	EXPECT_EQ(a += b, my_special_integer{3});
	EXPECT_EQ(a, my_special_integer{3});
}

TESTCASE(idis_integer_mixin_sub_assign)
{
	my_special_integer a{1};
	my_special_integer b{2};
	EXPECT_EQ(a -= b, my_special_integer{-1});
	EXPECT_EQ(a, my_special_integer{-1});
}

TESTCASE(idis_integer_mixin_div_assign)
{
	my_special_integer a{3};
	EXPECT_EQ(a /= 2, my_special_integer{1});
	EXPECT_EQ(a, my_special_integer{1});
}

TESTCASE(idis_integer_mixin_mul_assign)
{
	my_special_integer a{3};
	EXPECT_EQ(a *= 2, my_special_integer{6});
	EXPECT_EQ(a, my_special_integer{6});
}

TESTCASE(idis_integer_mixin_add)
{
	my_special_integer a{1};
	my_special_integer b{2};
	auto c = a + b;
	EXPECT_EQ(c, my_special_integer{3});
}

TESTCASE(idis_integer_mixin_sub)
{
	my_special_integer a{1};
	my_special_integer b{2};
	auto c = a - b;
	EXPECT_EQ(c, my_special_integer{-1});
}

TESTCASE(idis_integer_mixin_mul)
{
	my_special_integer a{2};
	auto c = 3 * a;
	EXPECT_EQ(c, my_special_integer{6});
}

TESTCASE(idis_integer_mixin_div)
{
	my_special_integer a{3};
	auto c = a / 2;
	EXPECT_EQ(c, my_special_integer{1});
}

TESTCASE(idis_integer_mixin_abs)
{
	my_special_integer a{-4};
	EXPECT_EQ(abs(a), my_special_integer{4});
}
