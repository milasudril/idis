#ifndef IDIS_EVENTSEQUENCER_ACTION_HPP
#define IDIS_EVENTSEQUENCER_ACTION_HPP

#include "./timestamp.hpp"

#include "error_handler/exception.hpp"

#include <memory>

namespace idis::seq
{
	namespace detail
	{
		struct action_data
		{
			using deleter = void (*)(void*);
			using owner   = std::unique_ptr<void, deleter>;

			owner pointer{nullptr, [](void*) {}};
			void (*callback)(void const*){[](void const*) {
				throw exception{"Called failed", "empty action"};
			}};
		};

		template<class T>
		action_data::owner make_type_erased_unique_ptr(T&& obj)
		{
			return action_data::owner{new T{std::forward<T>(obj)},
			                          [](void* data)
			                          {
				                          auto obj = static_cast<T*>(data);
				                          delete obj;
			                          }};
		}
	}

	class action
	{
	public:
		template<class Payload>
		explicit action(Payload&& data,
		                std::enable_if_t<!std::is_same_v<std::decay_t<Payload>, action>, int> = 0)
		    : m_data{detail::make_type_erased_unique_ptr(std::forward<Payload>(data)),
		             [](void const* data)
		             {
			             auto& obj = *static_cast<Payload const*>(data);
			             obj();
		             }}
		{
		}

		void operator()() const { m_data.callback(m_data.pointer.get()); }

	private:
		detail::action_data m_data;
	};
}

#endif