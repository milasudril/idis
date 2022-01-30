//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_instance.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_VKINIT_INSTANCE_HPP
#define IDIS_VKINIT_INSTANCE_HPP

#include "./initializer.hpp"

#include <memory>

namespace idis::wm
{
	struct instance_deleter
	{
		void operator()(VkInstance inst) const
		{
			if(inst != nullptr) { vkDestroyInstance(inst, nullptr); }
		}
	};

	class vk_instance
	{
	public:
		vk_instance();

		auto handle() const { return m_handle.get(); }

	private:
		[[no_unique_address]] initializer m_init;
		std::unique_ptr<std::remove_pointer_t<VkInstance>, instance_deleter> m_handle;
	};
}

#endif
