#ifndef IDIS_EVENTSEQUENCER_EVENT_HPP
#define IDIS_EVENTSEQUENCER_EVENT_HPP

#include "./timestamp.hpp"
#include "./action.hpp"

namespace idis::seq
{
	class event
	{
	public:
		explicit event(timestamp t, action&& action): m_expire_time{t}, m_action{std::move(action)}
		{
		}

		bool has_expired(timepoint t) const { return m_expire_time.frame() < t; }

		void fire() const { m_action(); }

		auto expire_time() const { return m_expire_time; }

	private:
		timestamp m_expire_time;
		action m_action;
	};
}

#endif