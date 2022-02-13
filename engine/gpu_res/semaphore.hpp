//@	{
//@	 "dependencies_extra": [{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_SEMAPHORE_HPP
#define IDIS_GPURES_SEMAPHORE_HPP

#include "engine/vk_init/device.hpp"
#include "engine/vk_init/error.hpp"
#include "engine/error_handling/exception.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class semaphore_deleter
	{
	public:
		explicit semaphore_deleter(VkDevice device): m_device{device} {}

		void operator()(VkSemaphore obj) const
		{
			if(obj != nullptr) { vkDestroySemaphore(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using semaphore_handle = std::unique_ptr<std::remove_pointer_t<VkSemaphore>, semaphore_deleter>;

	inline auto create_semaphore(VkDevice device)
	{
		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkSemaphore ret{};
		if(auto res = vkCreateSemaphore(device, &semaphore_info, nullptr, &ret); res != VK_SUCCESS)
		{
			throw idis::exception{"create semaphore", to_string(vk_init::error{res})};
		}

		return semaphore_handle{ret, semaphore_deleter{device}};
	}

	class semaphore
	{
	public:
		using handle_type = semaphore_handle;

		explicit semaphore(vk_init::device& device): semaphore{device.handle()} {}

		explicit semaphore(VkDevice device): m_handle{create_semaphore(device)} {}

		VkSemaphore handle() const { return m_handle.get(); }

		auto device() const { return m_handle.get_deleter().device(); }

	private:
		handle_type m_handle;
	};
}

#endif