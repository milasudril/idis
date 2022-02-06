#ifndef IDIS_GPURES_UTILS_HPP
#define IDIS_GPURES_UTILS_HPP

#include "./vk_swapchain.hpp"
#include "./vk_image_view.hpp"

namespace idis::gpu_res
{
	inline std::vector<vk_image_view> get_image_views_from(vk_swapchain const& src)
	{
		std::vector<vk_image_view> ret;
		std::ranges::transform(src.get_images(), std::back_inserter(ret), [dev = src.device(),
							   format = src.image_format()](auto item) {
			return vk_image_view{dev, item, format};
		});
		return ret;
	}
}

#endif