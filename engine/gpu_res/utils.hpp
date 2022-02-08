#ifndef IDIS_GPURES_UTILS_HPP
#define IDIS_GPURES_UTILS_HPP

#include "./swapchain.hpp"
#include "./image_view.hpp"

namespace idis::gpu_res
{
	inline std::vector<image_view> get_image_views_from(swapchain const& src)
	{
		std::vector<image_view> ret;
		std::ranges::transform(src.get_images(),
		                       std::back_inserter(ret),
		                       [dev = src.device(), format = src.image_format()](auto item) {
			                       return image_view{dev, item, format};
		                       });
		return ret;
	}
}

#endif