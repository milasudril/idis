//@	{"target":{"name":"glfw_stub.o"}}

#include "./glfw_stub.hpp"

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

	GLFWwindow* glfwCreateWindow(
	    int width, int height, char const* title, GLFWmonitor* monitor, GLFWwindow* share)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.create_window);
		return idis::wm::glfw_stub::call_if_not_nullptr(
		    std::pair{fptr, "create_window"}, width, height, title, monitor, share);
	}

	void glfwDestroyWindow(GLFWwindow* window)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.destroy_window);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "destroy_window"}, window);
	}

	void glfwWindowHint(int key, int value)
	{
		auto fptr = std::ref(idis::wm::glfw_stub::overrides.window_hint);
		return idis::wm::glfw_stub::call_if_not_nullptr(std::pair{fptr, "window_hint"}, key, value);
	}
}