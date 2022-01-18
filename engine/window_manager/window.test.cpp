//@	{"target":{"name":"window.test"}}

#include "./window.hpp"

#include "./glfw_stub.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_wm_window_create)
{
	idis::wm::glfw_stub::overrides.init = []() { return GLFW_TRUE; };
	idis::wm::glfw_stub::overrides.terminate = []() {};
	idis::wm::window win;
}