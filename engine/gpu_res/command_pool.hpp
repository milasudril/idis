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

	class command_buffer_set
	{
	public:
		command_buffer_set() = default;

		explicit command_buffer_set(command_pool& pool, size_t n);

		command_buffer_set(command_buffer_set const&) = delete;
		command_buffer_set& operator=(command_buffer_set const&) = delete;
		command_buffer_set(command_buffer_set&&)                 = default;
		command_buffer_set& operator=(command_buffer_set&&) = default;

		~command_buffer_set()
		{
			if(!m_storage.empty())
			{
				vkFreeCommandBuffers(m_owner->device(),
				                     m_owner->handle(),
				                     static_cast<uint32_t>(std::size(m_storage)),
				                     std::data(m_storage));
			}
		}

		std::span<VkCommandBuffer const> buffers() const { return m_storage; }

		VkCommandBuffer operator[](size_t index) const { return m_storage[index]; }

	private:
		std::vector<VkCommandBuffer> m_storage;
		command_pool const* m_owner;
	};
}

#endif