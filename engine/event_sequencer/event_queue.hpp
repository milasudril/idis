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

	class event_queue
	{
	public:
		template<class T>
		void push(timepoint expire_time, T&& event_data)
		{
			m_queue.push(event{timestamp{expire_time, m_event_index}, std::forward<T>(event_data)});
			m_event_index.next();
		}

		decltype(auto) top() const { return m_queue.top(); }

		void pop() { m_queue.pop(); }

		bool empty() const { return m_queue.empty(); }

	private:
		std::priority_queue<event, std::vector<event>, detail::event_comparator> m_queue;
		event_index m_event_index;
	};

	inline void drain(event_queue& queue, timepoint until)
	{
		while(!queue.empty() && queue.top().has_expired(until))
		{
			queue.top().fire();
			queue.pop();
		}
	}
}
#endif