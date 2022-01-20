#ifndef IDIS_EVENTSEQUENCER_ACTION_HPP
#define IDIS_EVENTSEQUENCER_ACTION_HPP

#include "./timestamp.hpp"

#include "error_handler/exception.hpp"

namespace idis::seq
{
	namespace detail
	{
		struct action_data
		{
			void* pointer{nullptr};
			void (*callback)(void const*){[](void const*) {
				    throw exception{"Called failed", "empty action"};
			    }};
			void (*dtor)(void*){[](void*) {}};
		};
	}

	class action
	{
	public:
		template<class Payload>
		explicit action(Payload&& data, std::enable_if_t<!std::is_same_v<std::decay_t<Payload>, action>, int> = 0)
		    : m_data{new Payload{std::forward<Payload>(data)},
		             [](void const* data)
		             {
			             auto& obj = *static_cast<Payload const*>(data);
			             obj();
		             },
		             [](void* data)
		             {
			             auto obj = static_cast<Payload*>(data);
			             delete obj;
		             }}
		{
		}

		action(action const&) = delete;

		action(action&& other) noexcept
		{
			m_data     = other.m_data;
			other.m_data = detail::action_data{};
		}

		action& operator=(action const&) = delete;

		action& operator=(action&& other) noexcept
		{
			m_data.dtor(m_data.pointer);
			m_data     = other.m_data;
			other.m_data = detail::action_data{};
			return *this;
		}

		~action() noexcept { m_data.dtor(m_data.pointer); }

		void operator()() const { m_data.callback(m_data.pointer); }

	private:
		detail::action_data m_data;
	};
}

#endif