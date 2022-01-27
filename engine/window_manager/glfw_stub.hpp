//@	{"dependencies_extra":[{"ref":"./glfw_stub.o", "rel":"implementation"}]}

#ifndef IDIS_WINDOWMANAGER_GLFWSTUB_HPP
#define IDIS_WINDOWMANAGER_GLFWSTUB_HPP

#include <GLFW/glfw3.h>

#include "ext_proj/testfwk/validation.hpp"
#include "ext_proj/testfwk/testsuite.hpp"

namespace idis::wm::glfw_stub
{
	template<class T>
	T make_default()
	{
		return T{};
	}

	template<>
	inline void make_default<void>()
	{
	}

	template<class T, class... Args>
	decltype(auto) call_if_not_nullptr(std::pair<std::reference_wrapper<T>, char const*> f,
	                                   Args&&... args)
	{
		auto func = f.first.get();
		if(func == nullptr)
		{
			fprintf(stderr, "Unexpected call to %s\n", f.second);
			TestFwk::currentTestcase->testcaseFailed();
			return make_default<decltype(func(std::forward<Args>(args)...))>();
		}

		f.first.get() = nullptr;

		return func(std::forward<Args>(args)...);
	}

	struct function_overrides
	{
		decltype(&glfwInit) init;
		decltype(&glfwTerminate) terminate;
		decltype(&glfwGetVersionString) get_version_string;
		decltype(&glfwGetError) get_error;
		decltype(&glfwCreateWindow) create_window;
		decltype(&glfwDestroyWindow) destroy_window;
		decltype(&glfwWindowHint) window_hint;
	};

	inline constinit function_overrides overrides{};
}

struct GLFWwindow
{
};

#endif