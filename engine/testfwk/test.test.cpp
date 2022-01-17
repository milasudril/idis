//@	{"target":{"name":"test.test"}}

#include "./testfwk.hpp"

TESTCASE(SuccessfulTestcase) { EXPECT_OP(1, ==, 1); }

#if 0
TESTCASE(FailingTestcase)
{
	EXPECT_OP(1, ==, 2);
}

TESTCASE(RequireFromNonVoidFunctionA)
{
	auto test = [](int val)
	{
		REQUIRE_NE(val, 0);
		return 1.0/val;
	};

	test(0);

	// Not run
	EXPECT_EQ(test(2), 0.5);
}
#endif

TESTCASE(RequireFromNonVoidFunctionB)
{
	auto test = [](int val)
	{
		REQUIRE_NE(val, 0);
		return 1.0 / val;
	};

	EXPECT_EQ(test(2), 0.5);
}