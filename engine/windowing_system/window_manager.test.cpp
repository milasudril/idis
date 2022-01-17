//@	{"target":{"name":"window_manager.test"}}

#include "./window_manager.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(window_manager_copy)
{
	using wm = idis::window_manager;
	EXPECT_EQ(wm::instance_count(), 0);
	{
		wm wm_a;
		EXPECT_EQ(wm::instance_count(), 1);
		{
			auto wm_b = wm_a;
			EXPECT_EQ(wm::instance_count(), 2);
		}
		EXPECT_EQ(wm::instance_count(), 1);

		auto wm_b = std::move(wm_a);
		EXPECT_EQ(wm::instance_count(), 1);
	}
	EXPECT_EQ(wm::instance_count(), 0);
}