//@{"dependencies_extra":[{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}]}

#ifndef IDIS_WINDOWINGSYSTEM_WINDOWMANAGER_HPP
#define IDIS_WINDOWINGSYSTEM_WINDOWMANAGER_HPP

#include "error_handler/exception.hpp"

#include <GLFW/glfw3.h>

namespace idis
{
	class window_manager
	{
	public:
		window_manager()
		{
			if(m_instance_count != 0)
			{
				++m_instance_count;
				return;
			}

			if(glfwInit() != GLFW_TRUE) [[unlikely]]
			{
				char const* cause = nullptr;
				glfwGetError(&cause);
				throw exception{"initialize the window manager",
				                cause != nullptr ? cause : "Unknown error"};
			}
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			++m_instance_count;
		}

		char const* version_string() const { return glfwGetVersionString(); }

		static constexpr char const* driver() { return "GLFW"; }

		static size_t instance_count() { return m_instance_count; }

		window_manager(window_manager const&) { ++m_instance_count; }

		window_manager(window_manager&&) noexcept = default;

		window_manager& operator=(window_manager const&)
		{
			++m_instance_count;
			return *this;
		}

		window_manager& operator=(window_manager&&) noexcept = default;

		~window_manager() noexcept
		{
			if(m_instance_count == 1)
			{ glfwTerminate(); }

			if(m_instance_count != 0)
			{ --m_instance_count; }
		}

	private:
		static size_t m_instance_count;
	};

	inline size_t window_manager::m_instance_count{0};
}

#endif