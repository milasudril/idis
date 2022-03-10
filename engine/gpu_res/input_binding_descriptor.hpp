#ifndef IDIS_GPURES_INPUTBINDINGDESCRIPTOR_HPP
#define IDIS_GPURES_INPUTBINDINGDESCRIPTOR_HPP

#include "./format_to_format_id.hpp"

#include "engine/utils/empty.hpp"

#include <vulkan/vulkan.h>

#include <array>
#include <tuple>

namespace idis::gpu_res
{
	template<size_t K, class Tuple>
	constexpr void build(empty<Tuple>,
	                     std::array<VkVertexInputBindingDescription, std::tuple_size_v<Tuple>>& ret)
	{
		if constexpr(K != 0)
		{
			ret[K - 1] = VkVertexInputBindingDescription{
			    K - 1, sizeof(std::tuple_element_t<K - 1, Tuple>), VK_VERTEX_INPUT_RATE_VERTEX};
			build<K - 1>(empty<Tuple>{}, ret);
		}
	}

	template<class Tuple>
	constexpr std::array<VkVertexInputBindingDescription, std::tuple_size_v<Tuple>> get_bindings(
	    empty<Tuple>)
	{
		std::array<VkVertexInputBindingDescription, std::tuple_size_v<Tuple>> ret{};
		build<std::tuple_size_v<Tuple>>(empty<Tuple>{}, ret);
		return ret;
	}

	template<size_t K, class Tuple>
	constexpr void build(
	    empty<Tuple>, std::array<VkVertexInputAttributeDescription, std::tuple_size_v<Tuple>>& ret)
	{
		if constexpr(K != 0)
		{
			ret[K - 1] = VkVertexInputAttributeDescription{
			    K - 1, K - 1, format_id_v<std::tuple_element_t<K - 1, Tuple>>, 0};
			build<K - 1>(empty<Tuple>{}, ret);
		}
	}

	template<class Tuple>
	constexpr std::array<VkVertexInputAttributeDescription, std::tuple_size_v<Tuple>>
	    get_attributes(empty<Tuple>)
	{
		std::array<VkVertexInputAttributeDescription, std::tuple_size_v<Tuple>> ret{};
		build<std::tuple_size_v<Tuple>>(empty<Tuple>{}, ret);
		return ret;
	}

	template<class ShaderDescriptor>
	struct input_binding_descriptor
	{
		using port_types                 = typename ShaderDescriptor::input_port_types;
		static constexpr auto num_inputs = std::tuple_size_v<port_types>;
		static constexpr std::array<VkVertexInputBindingDescription, num_inputs> bindings =
		    get_bindings(empty<port_types>{});
		static constexpr std::array<VkVertexInputAttributeDescription, num_inputs> attributes =
		    get_attributes(empty<port_types>{});
	};
}

#endif