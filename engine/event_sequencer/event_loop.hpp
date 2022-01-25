#ifndef IDIS_EVENTSEQUENCER_EVENTLOOP_HPP
#define IDIS_EVENTSEQUENCER_EVENTLOOP_HPP

#include "./event_queue.hpp"

namespace idis::seq
{

	class event_loop_state
	{
	public:
		bool should_exit() const { return m_should_exit; }

		void set_exit_flag() { m_should_exit = true; }

		template<class T>
		void push_event(timepoint expire_time, T&& event_data)
		{
			m_queue.queue.push(expire_time, std::forward<T>(event_data));
		}

		timepoint now() const { return m_now; }

	protected:
		timepoint m_now;
		bool m_should_exit{false};
		event_queue m_queue;
	};

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
			internal_state.drain_queue();
			m_post_peek_callback();
			m_state.tick();
		}

		event_loop_state& state() const { return m_state; }

	private:
		class internal_state: public event_loop_state
		{
		public:
			void tick() { m_now.next(); }

			void drain_queue() { drain(m_queue, m_now); }
		};

		internal_state m_state;
		action m_pre_peek_callback;
		action m_post_peek_callback;
	};
}

#endif
