//@	{"target":{"name":"initializer.test"}}

#include "./glfw_stub.hpp"

#include "./initializer.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

#include <cstring>

TESTCASE(idis_wm_initializer_copy)
{
	using init = idis::wm::initializer;
	EXPECT_EQ(init::instance_count(), 0);
	{
		idis::wm::glfw_stub::overrides.init = []() { return GLFW_TRUE; };
		init init_a;
		EXPECT_EQ(init::instance_count(), 1);
		{
			auto init_b = init_a;
			EXPECT_EQ(init::instance_count(), 2);
		}
		EXPECT_EQ(init::instance_count(), 1);

		auto init_b = std::move(init_a);
		EXPECT_EQ(init::instance_count(), 1);

		idis::wm::glfw_stub::overrides.terminate = []() {};
	}
	EXPECT_EQ(init::instance_count(), 0);
}

TESTCASE(idis_wm_initializer_create_two)
{
	using init = idis::wm::initializer;
	EXPECT_EQ(init::instance_count(), 0);
	{
		idis::wm::glfw_stub::overrides.init = []() { return GLFW_TRUE; };
		init init_a;
		EXPECT_EQ(init::instance_count(), 1);
		{
			init init_b;
			EXPECT_EQ(init::instance_count(), 2);
		}
		EXPECT_EQ(init::instance_count(), 1);
		idis::wm::glfw_stub::overrides.terminate = []() {};
	}
	EXPECT_EQ(init::instance_count(), 0);
}
#if 0
TESTCASE(idis_wm_initializer_fail_init)
{
	using init = idis::wm::initializer;
	EXPECT_EQ(init::instance_count(), 0);
	idis::wm::glfw_stub::overrides.init      = []() { return GLFW_FALSE; };
	idis::wm::glfw_stub::overrides.get_error = [](char const** msg)
	{
		*msg = static_cast<char const*>("Foobar");
		return 0;
	};

	try
	{
		init init_a;
		TestFwk::currentTestcase->testcaseFailed();
	}
	catch(idis::exception const& e)
	{
		EXPECT_EQ(strcmp(e.what(), "Failed to initialize the window manager: Foobar"), 0);
	}
}
#endif