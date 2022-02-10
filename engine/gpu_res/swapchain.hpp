//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./swapchain.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKSWAPCHAIN_HPP
#define IDIS_GPURES_VKSWAPCHAIN_HPP

#include "engine/vk_init/device.hpp"
#include "engine/vk_init/surface.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class swapchain_deleter
	{
	public:
		explicit swapchain_deleter(VkDevice device): m_device{device} {}

		void operator()(VkSwapchainKHR obj) const
		{
			if(obj != nullptr) { vkDestroySwapchainKHR(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	class swapchain
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkSwapchainKHR>, swapchain_deleter>;

		swapchain() = default;

		explicit swapchain(init::device& device, init::surface& surface);

		VkSwapchainKHR handle() const { return m_data.first.get(); }

		std::vector<VkImage> get_images() const
		{
			uint32_t img_count{};
			vkGetSwapchainImagesKHR(
			    m_data.first.get_deleter().device(), m_data.first.get(), &img_count, nullptr);
			std::vector<VkImage> ret(img_count);
			vkGetSwapchainImagesKHR(m_data.first.get_deleter().device(),
			                        m_data.first.get(),
			                        &img_count,
			                        std::data(ret));
			return ret;
		}

		auto device() const { return m_data.first.get_deleter().device(); }

		auto image_format() const { return m_data.second; }

	private:
		std::pair<handle_type, VkFormat> m_data;
	};
}

#endif