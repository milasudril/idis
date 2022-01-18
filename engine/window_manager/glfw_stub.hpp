#ifndef IDIS_WINDOWMANAGER_GLFWSTUB_HPP
#define IDIS_WINDOWMANAGER_GLFWSTUB_HPP

#include <GLFW/glfw3.h>

#include "testfwk/validation.hpp"
#include "testfwk/testsuite.hpp"

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
	};

	inline constinit function_overrides overrides{};
}

extern "C"
{
	inline int glfwInit()
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.init);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "init"});
	}

	inline void glfwTerminate()
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.terminate);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "terminate"});
	}

	inline char const* glfwGetVersionString()
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.get_version_string);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "get_version_string"});
	}

	inline int glfwGetError(char const** msg)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.get_error);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "get_error"}, msg);
	}

	inline GLFWwindow* glfwCreateWindow(
	    int width, int height, char const* title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.create_window);
		return idis::wm::glfw_stub::call_if_not_nullptr(
		    std::pair{fptr, "create_window"}, width, height, title, monitor, share);
	}

	inline void glfwDestroyWindow(GLFWwindow* window)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.destroy_window);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "destroy_window"}, window);
	}
}

struct GLFWwindow
{
};

#endif