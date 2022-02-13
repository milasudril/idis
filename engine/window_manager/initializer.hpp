//@	{"dependencies_extra":[{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}]}

#ifndef IDIS_WINDOWMANAGER_INITIALIZER_HPP
#define IDIS_WINDOWMANAGER_INITIALIZER_HPP

#include "engine/error_handling/exception.hpp"
#include <GLFW/glfw3.h>

namespace idis::wm
{
	class initializer
	{
	public:
		initializer()
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

			++m_instance_count;
		}

		static size_t instance_count() { return m_instance_count; }

		initializer(initializer const&) { ++m_instance_count; }

		initializer(initializer&&) noexcept = default;

		initializer& operator=(initializer const&)
		{
			++m_instance_count;
			return *this;
		}

		initializer& operator=(initializer&&) noexcept = default;

		~initializer() noexcept
		{
			if(m_instance_count == 1)
			{
				try
				{
					glfwTerminate();
				}
				catch(...)
				{
				}
			}

			if(m_instance_count != 0) { --m_instance_count; }
		}

	private:
		static size_t m_instance_count;
	};

	inline constinit size_t initializer::m_instance_count{0};
}

#endif