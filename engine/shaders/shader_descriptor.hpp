#ifndef IDIS_SHADERS_SHADERSDESCRIPTOR_HPP
#define IDIS_SHADERS_SHADERSDESCRIPTOR_HPP

#ifdef __cplusplus
	#include "./shader_source.hpp"
	#include "engine/utils/vec_t.hpp"
	#include <tuple>
	#include <span>

	#define IDIS_BEGIN_SHADER_DESCRIPTOR(namespace_name, name)                                     \
		namespace namespace_name                                                                   \
		{                                                                                          \
			struct name                                                                            \
			{                                                                                      \
				using port_types = std::tuple <

	#define IDIS_SHADER_INPUT(type, name) ::idis::type##f_t,

	#define IDIS_END_SHADER_DESCRIPTOR()                                                           \
		nullptr_t > ;                                                                              \
		static ::idis::shaders::vertex_shader_source<std::span<uint32_t const>> vertex_shader();   \
		static ::idis::shaders::fragment_shader_source<std::span<uint32_t const>>                  \
		fragment_shader();                                                                         \
                                                                                                   \
		static constexpr auto num_inputs = std::tuple_size_v<port_types> - 1;                      \
		}                                                                                          \
		;                                                                                          \
		}

#else
const int port_counter_base = __COUNTER__;
	#define IDIS_BEGIN_SHADER_DESCRIPTOR(namespace_name, name)
	#define IDIS_SHADER_INPUT(type, name)                                                          \
		layout(location = __COUNTER__ - port_counter_base - 1) in type name;
	#define IDIS_END_SHADER_DESCRIPTOR()
#endif

#endif