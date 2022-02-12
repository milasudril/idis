#ifndef IDIS_VKINIT_ERROR_HPP
#define IDIS_VKINIT_ERROR_HPP

#include "engine/helpers/case_ret_str.hpp"

#include <vulkan/vulkan.h>

namespace idis::vk_init
{
	class error
	{
	public:
		explicit error(VkResult value): m_value{value}
		{}

		auto value() const { return m_value; }

	private:
		VkResult m_value;
	};

	inline std::string to_string(error e)
	{
		switch(e.value())
		{
			CASE_RET_STR(VK_SUCCESS);
			CASE_RET_STR(VK_ERROR_OUT_OF_HOST_MEMORY);
			CASE_RET_STR(VK_ERROR_OUT_OF_DEVICE_MEMORY);
			CASE_RET_STR(VK_ERROR_DEVICE_LOST);
			CASE_RET_STR(VK_ERROR_SURFACE_LOST_KHR);
			CASE_RET_STR(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
			CASE_RET_STR(VK_ERROR_INITIALIZATION_FAILED);
			default:
				return "Unknown error";
		}
	}
}

#endif