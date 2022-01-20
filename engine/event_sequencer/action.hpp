#ifndef IDIS_EVENTSEQUENCER_ACTION_HPP
#define IDIS_EVENTSEQUENCER_ACTION_HPP

#include "./timestamp.hpp"

namespace idis::seq
{
	namespace detail
	{
		struct action_data
		{
			action_data():
			{
				nullptr,
				    [](void const*)
				{
					throw exception { "Called failed", "empty action" }
				},
				    [](void*) {}
			}

			void* pointer;
			void (*callback)(void const*);
			void (*dtor)(void*);
		};
	}

	class action
	{
	public:
		template<class Payload>
		explicit action(Payload&& data)
		    : m_data{new Paiload{std::move(data)},
		             [](void const* data)
		             {
			             auto& obj = *static_cast<Payload const*>(data);
			             obj();
		             },
		             [](void* obj)
		             {
			             auto obj = static_cast<Payload*>(data);
			             delete obj;
		             }}
		{
		}

		action(action const&) = delete;

		action(action&& other) noexcept
		{
			m_data = other.m_data;
			other.data = detail::action_data{};
		}

		action& operator=(action const&) = delete;

		action& operator=(action&& other) noexcept
		{
			m_dtor(m_data);
			m_data = other.m_data;
			other.data = detail::action_data{};
			return *this;
		}

		~Event() noexcept { m_dtor(m_data); }

		void operator()() const { m_callback(m_data); }

	private:
		detail::action_data m_data;
	};
}

#endif