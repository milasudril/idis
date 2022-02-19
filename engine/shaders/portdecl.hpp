#ifndef IDIS_SHADERS_PORTDECL_HPP
#define IDIS_SHADERS_PORTDECL_HPP

#ifdef __cplusplus
	#include "engine/utils/vec_t.hpp"

	#define IDIS_BEGIN_SHADER_DESCRIPTOR(namespace_name, name)                                     \
		namespace namespace_name                                                                   \
		{                                                                                          \
			struct name                                                                            \
			{                                                                                      \
				static constexpr uint32_t port_counter_base = __COUNTER__;

	#define IDIS_SHADER_INPUT(type, name)                                                          \
		struct name                                                                                \
		{                                                                                          \
			static constexpr uint32_t port = __COUNTER__ - port_counter_base - 1;                  \
			using element_type             = type##f_t;                                            \
		}


	#define IDIS_END_SHADER_DESCRIPTOR()                                                           \
		}                                                                                          \
		;                                                                                          \
		}

#else
const int port_counter_base = __COUNTER__;
	#define IDIS_BEGIN_SHADER_DESCRIPTOR(namespace_name, name)
	#define IDIS_SHADER_INPUT(type, name)                                                          \
		layout(location = __COUNTER__ - port_counter_base - 1) in type name
	#define IDIS_END_SHADER_DESCRIPTOR()
#endif

#endif