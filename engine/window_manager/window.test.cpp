//@	{"target":{"name":"window.test"}}

#include "./window.hpp"

#include "./glfw_stub.hpp"

#include "testfwk/testfwk.hpp"

#include <cstring>

TESTCASE(idis_wm_window_create)
{
	idis::wm::glfw_stub::overrides.init      = []() { return GLFW_TRUE; };
	idis::wm::glfw_stub::overrides.terminate = []() {};
	idis::wm::glfw_stub::overrides.window_hint = [](int key, int value) {
		EXPECT_EQ(key, GLFW_CLIENT_API);
		EXPECT_EQ(value, GLFW_NO_API);
	};
	idis::wm::glfw_stub::overrides.create_window =
	    [](int width, int height, char const* title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		EXPECT_EQ(width, 800);
		EXPECT_EQ(height, 500);
		EXPECT_EQ(strcmp(title, "Foobar"), 0);
		EXPECT_EQ(monitor, nullptr);
		EXPECT_EQ(share, nullptr);

		return new GLFWwindow{};
	};

	idis::wm::window win{800, 500, "Foobar"};

	idis::wm::glfw_stub::overrides.destroy_window = [](GLFWwindow* window) { delete window; };
}