#ifndef IDIS_GPURES_TYPETOTYPEID_HPP
#define IDIS_GPURES_TYPETOTYPEID_HPP

#include "engine/utils/vec_t.hpp"

#include <vulkan/vulkan.h>

namespace idis::gpu_res
{
	template<class T>
	constexpr auto format_id_v = VK_FORMAT_UNDEFINED;

	template<auto TypeId>
	struct format;

	template<auto TypeId>
	using format_t = format<TypeId>::type;


	template<>
	constexpr auto format_id_v<vec2i_t> = VK_FORMAT_R32G32_SINT;

	template<>
	struct format<VK_FORMAT_R32G32_SINT>
	{
		using type = vec2i_t;
	};

	template<>
	constexpr auto format_id_v<vec4i_t> = VK_FORMAT_R32G32B32A32_SINT;

	template<>
	struct format<VK_FORMAT_R32G32B32A32_SINT>
	{
		using type = vec4i_t;
	};

	template<>
	constexpr auto format_id_v<vec2f_t> = VK_FORMAT_R32G32_SFLOAT;

	template<>
	struct format<VK_FORMAT_R32G32_SFLOAT>
	{
		using type = vec2f_t;
	};

	template<>
	constexpr auto format_id_v<vec4f_t> = VK_FORMAT_R32G32B32A32_SFLOAT;

	template<>
	struct format<VK_FORMAT_R32G32B32A32_SFLOAT>
	{
		using type = vec4f_t;
	};
}

#endif