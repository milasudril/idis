#ifndef IDIS_EVENTSEQUENCER_EVENTLOOP_HPP
#define IDIS_EVENTSEQUENCER_EVENTLOOP_HPP

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

	class event_queue
	{
	public:
		template<class T>
		void push(T&& event_data, timepoint expire_time)
		{
			m_pending_events.push(event{timestamp{expire_time, m_event_index}, std::forward<T>(event_data)});
			++m_event_index;
		}

		decltype(auto) top() const
		{
			return m_queue.top();
		}

		void pop()
		{
			m_queue.pop();
		}

		bool empty() const
		{
			return m_queue.empty();
		}

	private:
		std::priority_queue<event, std::vector<event>, detail::event_comparator> m_queue;
		event_index m_event_index;
	};

	inline void drain(evnet_queue& queue, timepoint until)
	{
		while(!m_pending_events.empty() && m_pending_events.top().has_expired(until))
		{
			m_pending_events.top().fire();
			m_pending_events.pop();
		}
	}

	class event_loop
	{
	public:
		void run()
		{
			while(!m_should_exit)
			{
				do_iteration();
			}
		}

		void do_iteration()
		{
			m_pre_peek_callback();
			drain(m_pending_events, m_now);
			m_post_peek_callback();
			m_now.next();
		}

	private:
		bool m_should_exit;
		action m_pre_peek_callback;
		event_queue m_pending_events;
		action m_post_peek_callback;
		timepoint m_now;
	};
}

#endif
