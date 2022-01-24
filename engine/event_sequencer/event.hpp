#ifndef IDIS_EVENTSEQUENCER_EVENT_HPP
#define IDIS_EVENTSEQUENCER_EVENT_HPP

#include "./timestamp.hpp"
#include "./action.hpp"

namespace idis::seq
{
	class event
	{
	public:
		template<class T>
		explicit event(timestamp t, T&& action_data)
		    : m_expire_time{t}
		    , m_action{std::make_unique<action<T>>(std::forward<T>(action_data))}
		{
		}

		bool has_expired(timepoint t) const { return m_expire_time.frame() <= t; }

		void fire() const { (*m_action)(); }

		auto expire_time() const { return m_expire_time; }

	private:
		timestamp m_expire_time;
		std::unique_ptr<abstract_action> m_action;
	};
}

#endif
