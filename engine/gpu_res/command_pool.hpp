//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./command_pool.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_COMMANDPOOL_HPP
#define IDIS_GPURES_COMMANDPOOL_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class command_pool_deleter
	{
	public:
		explicit command_pool_deleter(VkDevice device): m_device{device} {}

		void operator()(VkCommandPool obj) const
		{
			if(obj != nullptr) { vkDestroyCommandPool(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class command_pool
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkCommandPool>, command_pool_deleter>;

		command_pool(): m_handle{nullptr, command_pool_deleter{nullptr}} {}

		explicit command_pool(vk_init::device& device);

		VkCommandPool handle() const { return m_handle.get(); }

		auto device() const { return m_handle.get_deleter().device(); }

	private:
		handle_type m_handle;
	};
}

#endif