#ifndef IDIS_EVENTSEQUENCER_EVENTQUEUE_HPP
#define IDIS_EVENTSEQUENCER_EVENTQUEUE_HPP

#include "./event.hpp"

#include <queue>

namespace idis::seq
{
	namespace detail
	{
		struct event_comparator
		{
			bool operator()(event const& a, event const& b) const
			{
				return a.expire_time() > b.expire_time();
			}
		};
	}

	using event_queue = std::priority_queue<event, std::vector<event>, detail::event_comparator>;

	class frame_processor
	{
	public:
		bool operator()(timepoint t)
		{
			m_pre_peek_callback();
			while(!m_pending_events.empty() && m_pending_events.top().has_expired(t))
			{
				m_pending_events.top().fire();
				m_pending_events.pop();
			}
			m_post_peek_callback();
		}

	private:
		event_queue m_pending_events;
		action m_pre_peek_callback;
		action m_post_peek_callback;
	};
}

#endif