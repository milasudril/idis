//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./swapchain.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_SWAPCHAIN_HPP
#define IDIS_GPURES_SWAPCHAIN_HPP

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

		swapchain()
		    : m_data{handle_type{nullptr, swapchain_deleter{nullptr}}, VkFormat{}, VkExtent2D{}}
		{
		}

		explicit swapchain(std::reference_wrapper<vk_init::device const> device,
		                   std::reference_wrapper<vk_init::surface const> surface);

		VkSwapchainKHR handle() const { return std::get<0>(m_data).get(); }

		auto device() const { return std::get<0>(m_data).get_deleter().device(); }

		auto image_format() const { return std::get<1>(m_data); }

		auto extent() const { return std::get<2>(m_data); }

		void reset() { std::get<0>(m_data).reset(); }

	private:
		std::tuple<handle_type, VkFormat, VkExtent2D> m_data;
	};

	inline auto get_images(std::reference_wrapper<swapchain const> swp)
	{
		uint32_t img_count{};
		vkGetSwapchainImagesKHR(swp.get().device(), swp.get().handle(), &img_count, nullptr);
		varlength_array<VkImage> ret{img_count};
		vkGetSwapchainImagesKHR(swp.get().device(), swp.get().handle(), &img_count, std::data(ret));
		return ret;
	}
}

#endif