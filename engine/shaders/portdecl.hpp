#ifndef IDIS_SHADERS_PORTDECL_HPP
#define IDIS_SHADERS_PORTDECL_HPP

#ifdef __cplusplus
	#include "engine/utils/vec_t.hpp"

	#define IDIS_SHADER_INPUT(loc, type, name)                                                     \
		struct name                                                                                \
		{                                                                                          \
			static constexpr uint32_t port = loc;                                                  \
			using element_type             = type##f_t;                                            \
		}

	#define IDIS_BEGIN_SHADER_DESCRIPTOR(namespace_name, name)                                     \
		namespace namespace_name                                                                   \
		{                                                                                          \
			struct name                                                                            \
			{

	#define IDIS_END_SHADER_DESCRIPTOR()                                                           \
		}                                                                                          \
		;                                                                                          \
		}

#else
	#define IDIS_SHADER_INPUT(loc, type, name) layout(location = loc) in type name
	#define IDIS_BEGIN_SHADER_DESCRIPTOR(namespace_name, name)
	#define IDIS_END_SHADER_DESCRIPTOR()
#endif

#endif