#ifndef IDIS_SHADERS_SHADERDATA_HPP
#define IDIS_SHADERS_SHADERDATA_HPP

#include <vulkan/vulkan.h>

#include <cstdint>
#include <type_traits>

namespace idis::shaders
{
	template<class Container, auto Type>
	requires(std::is_same_v<typename Container::value_type, uint32_t>) struct shader_source
	{
		static constexpr auto type = Type;
		Container data;
	};

	template<class Container, auto Type>
	shader_source(Container) -> shader_source<Container, Type>;

	template<class Container>
	using vertex_shader_source = shader_source<Container, VK_SHADER_STAGE_VERTEX_BIT>;

	template<class Container>
	using fragment_shader_source = shader_source<Container, VK_SHADER_STAGE_FRAGMENT_BIT>;
}


#endif