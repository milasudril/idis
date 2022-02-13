//@	{
//@	 "dependencies_extra": [{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_FENCE_HPP
#define IDIS_GPURES_FENCE_HPP

#include "engine/vk_init/device.hpp"
#include "engine/vk_init/error.hpp"
#include "engine/error_handling/exception.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class fence_deleter
	{
	public:
		explicit fence_deleter(VkDevice device): m_device{device} {}

		void operator()(VkFence obj) const
		{
			if(obj != nullptr) { vkDestroyFence(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using fence_handle = std::unique_ptr<std::remove_pointer_t<VkFence>, fence_deleter>;

	inline auto create_fence(VkDevice device)
	{
		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		VkFence ret{};
		if(auto res = vkCreateFence(device, &fence_info, nullptr, &ret); res != VK_SUCCESS)
		{
			throw idis::exception{"create fence", to_string(idis::vk_init::error{res})};
		}

		return fence_handle{ret, fence_deleter{device}};
	}

	class fence
	{
	public:
		using handle_type = fence_handle;

		explicit fence(std::reference_wrapper<vk_init::device const> device)
		    : fence{device.get().handle()}
		{
		}

		explicit fence(VkDevice device): m_handle{create_fence(device)} {}

		VkFence handle() const { return m_handle.get(); }

		auto device() const { return m_handle.get_deleter().device(); }

	private:
		handle_type m_handle;
	};
}

#endif