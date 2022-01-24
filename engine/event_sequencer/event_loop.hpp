#ifndef IDIS_EVENTSEQUENCER_EVENTLOOP_HPP
#define IDIS_EVENTSEQUENCER_EVENTLOOP_HPP

#include "./event_queue.hpp"

namespace idis::seq
{

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
