#ifndef IDIS_VKINIT_ERROR_HPP
#define IDIS_VKINIT_ERROR_HPP

#include "engine/utils/case_ret_str.hpp"
#include "engine/error_handling/common_causes.hpp"

#include <vulkan/vulkan.h>

namespace idis::vk_init
{
	class error
	{
	public:
		explicit error(VkResult value): m_value{value} {}

		auto value() const { return m_value; }

	private:
		VkResult m_value;
	};

	inline char const* to_string(error e)
	{
		switch(e.value())
		{
			CASE_RET_STR(VK_SUCCESS);
			CASE_RET_STR(VK_ERROR_INVALID_SHADER_NV);
			CASE_RET_STR(VK_ERROR_LAYER_NOT_PRESENT);
			CASE_RET_STR(VK_ERROR_INCOMPATIBLE_DRIVER);
			CASE_RET_STR(VK_ERROR_EXTENSION_NOT_PRESENT);
			CASE_RET_STR(VK_ERROR_FEATURE_NOT_PRESENT);
			CASE_RET_STR(VK_ERROR_TOO_MANY_OBJECTS);
			CASE_RET_STR(VK_ERROR_OUT_OF_HOST_MEMORY);
			CASE_RET_STR(VK_ERROR_OUT_OF_DEVICE_MEMORY);
			CASE_RET_STR(VK_ERROR_DEVICE_LOST);
			CASE_RET_STR(VK_ERROR_SURFACE_LOST_KHR);
			CASE_RET_STR(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
			CASE_RET_STR(VK_ERROR_INITIALIZATION_FAILED);
			CASE_RET_STR(VK_SUBOPTIMAL_KHR);
			CASE_RET_STR(VK_ERROR_OUT_OF_DATE_KHR);
			CASE_RET_STR(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
			default: return idis::unknown_error;
		}
	}
}

#endif