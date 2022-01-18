#ifndef IDIS_WINDOWMANAGER_GLFWSTUB_HPP
#define IDIS_WINDOWMANAGER_GLFWSTUB_HPP

#include <GLFW/glfw3.h>

#include "testfwk/validation.hpp"

namespace idis::wm::glfw_stub
{
	template<class T, class... Args>
	decltype(auto) call_if_not_nullptr(std::pair<std::reference_wrapper<T>, char const*> f,
	                                   Args&&... args)
	{
		if(f.first == nullptr)
		{
			throw std::runtime_error{std::string{"Unexpected call to "}.append(f.second)};
		}

		auto func     = f.first.get();
		f.first.get() = nullptr;

		return func(std::forward<Args>(args)...);
	}

	struct function_overrides
	{
		decltype(&glfwInit) init;
		decltype(&glfwTerminate) terminate;
		decltype(&glfwGetVersionString) get_version_string;
		decltype(&glfwGetError) get_error;
	};

	inline constinit function_overrides overrides{};
}

extern "C"
{
	int glfwInit()
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.init);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "init"});
	}

	void glfwTerminate()
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.terminate);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "terminate"});
	}

	char const* glfwGetVersionString()
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.get_version_string);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "get_version_string"});
	}

	int glfwGetError(char const** msg)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.get_error);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "get_error"}, msg);
	}
}
#endif