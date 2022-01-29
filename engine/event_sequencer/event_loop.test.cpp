//@	{"target":{"name":"event_loop.test"}}

#include "./event_loop.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_seq_event_loop_state_set_exit_flag)
{
	idis::seq::event_loop_state state;
	EXPECT_EQ(state.should_exit(), false);
	state.set_exit_flag();
	EXPECT_EQ(state.should_exit(), true);
}

TESTCASE(idis_seq_event_loop_state_push_event)
{
	idis::seq::event_loop_state state;
	EXPECT_EQ(state.queue_is_empty(), true);
	state.push_event(idis::seq::timepoint{}, []() {});
	EXPECT_EQ(state.queue_is_empty(), false);
}

TESTCASE(idis_seq_event_loop_state_initial_time)
{
	idis::seq::event_loop_state state;
	EXPECT_EQ(state.now().time_since_epoch().count(), 0);
}

TESTCASE(idis_seq_event_loop_do_iteration_no_cb)
{
	idis::seq::event_loop loop;
	auto t0 = loop.state().now();
	loop.do_iteration();
	auto t1 = loop.state().now();
	EXPECT_EQ(t1 - t0, idis::seq::tick{1});
}

TESTCASE(idis_seq_event_loop_do_iteration_with_cb)
{
	idis::seq::event_loop loop;
	int a = 0;
	int b = 0;
	loop.set_pre_drain_callback([&a]() { a = 1; });
	loop.set_post_drain_callback([&b, &a]() { b = a + 1; });
	auto t0 = loop.state().now();
	loop.do_iteration();
	auto t1 = loop.state().now();
	EXPECT_EQ(t1 - t0, idis::seq::tick{1});
	EXPECT_EQ(a, 1);
	EXPECT_EQ(b, 2);
}

TESTCASE(idis_seq_event_loop_three_iterations)
{
	idis::seq::event_loop loop;
	int iter_count = 0;
	loop.set_post_drain_callback(
	    [&state = loop.state(), &iter_count]()
	    {
		    if(state.now().time_since_epoch() == idis::seq::tick{2}) { state.set_exit_flag(); }
		    ++iter_count;
	    });
	auto t0 = loop.state().now();
	loop.run();
	EXPECT_EQ(iter_count, 3);
	auto t1 = loop.state().now();
	EXPECT_EQ(t1 - t0, idis::seq::tick{3});
}

TESTCASE(idis_seq_event_loop_three_iterations_one_event_left)
{
	idis::seq::event_loop loop;
	auto a = false;
	auto b = false;
	loop.state().push_event(idis::seq::timepoint{idis::seq::tick{1}}, [&a]() { a = true; });
	loop.state().push_event(idis::seq::timepoint{idis::seq::tick{3}}, [&b]() { b = true; });
	int iter_count = 0;
	loop.set_post_drain_callback(
	    [&state = loop.state(), &iter_count]()
	    {
		    if(state.now().time_since_epoch() == idis::seq::tick{2}) { state.set_exit_flag(); }
		    ++iter_count;
	    });
	auto t0 = loop.state().now();
	loop.run();
	EXPECT_EQ(iter_count, 3);
	auto t1 = loop.state().now();
	EXPECT_EQ(t1 - t0, idis::seq::tick{3});
	EXPECT_EQ(a, true);
	EXPECT_EQ(b, false);
}