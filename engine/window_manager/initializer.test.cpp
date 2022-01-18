//@	{"target":{"name":"initializer.test"}}

#include "./initializer.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(window_manager_copy)
{
	using init = idis::wm::initializer;
	EXPECT_EQ(init::instance_count(), 0);
	{
		init init_a;
		EXPECT_EQ(init::instance_count(), 1);
		{
			auto init_b = init_a;
			EXPECT_EQ(init::instance_count(), 2);
		}
		EXPECT_EQ(init::instance_count(), 1);

		auto init_b = std::move(init_a);
		EXPECT_EQ(init::instance_count(), 1);
	}
	EXPECT_EQ(init::instance_count(), 0);
}

TESTCASE(window_manager_create_two)
{
	using init = idis::wm::initializer;
	EXPECT_EQ(init::instance_count(), 0);
	{
		init init_a;
		EXPECT_EQ(init::instance_count(), 1);
		{
			init init_b;
			EXPECT_EQ(init::instance_count(), 2);
		}
		EXPECT_EQ(init::instance_count(), 1);
	}
	EXPECT_EQ(init::instance_count(), 0);
}