//@{"target":{"name":"timepoint.test"}}

#include "./timepoint.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

TESTCASE(idis_seq_tick_to_string_neg)
{
	auto const time_difference = idis::seq::tick{-70};
	auto const str             = to_string(time_difference);
	EXPECT_EQ(str, "-1:10");
}

TESTCASE(idis_seq_tick_to_string_rem_less_than_ten)
{
	auto const time_difference = idis::seq::tick{5};
	auto const str             = to_string(time_difference);
	EXPECT_EQ(str, "0:05");
}

TESTCASE(idis_seq_ticks_to_seconds)
{
	auto const ticks   = idis::seq::tick{15};
	auto const seconds = to_seconds(ticks);
	EXPECT_EQ(seconds.count(), std::chrono::duration<double>{0.25}.count());
}

TESTCASE(idis_seq_ticks_from_seconds)
{
	auto const seconds = std::chrono::duration<double>{0.25};
	auto const ticks   = from(idis::empty<idis::seq::tick>{}, seconds);
	EXPECT_EQ(ticks, idis::seq::tick{15});
}

TESTCASE(idis_seq_timepoint_step)
{
	idis::seq::timepoint start{idis::seq::tick{1}};
	EXPECT_EQ(start.time_since_epoch(), idis::seq::tick{1});
	start.next();
	EXPECT_EQ(start, idis::seq::timepoint{idis::seq::tick{2}});
}

TESTCASE(idis_seq_timepoint_add_offset)
{
	idis::seq::timepoint start{idis::seq::tick{1}};
	EXPECT_EQ(start.time_since_epoch(), idis::seq::tick{1});
	EXPECT_EQ(start + idis::seq::tick{50}, idis::seq::timepoint{idis::seq::tick{51}});
}

TESTCASE(idis_seq_timepoint_subtract_offset)
{
	idis::seq::timepoint start{idis::seq::tick{1}};
	EXPECT_EQ(start.time_since_epoch(), idis::seq::tick{1});
	EXPECT_EQ(start - idis::seq::tick{50}, idis::seq::timepoint{idis::seq::tick{-49}});
}

TESTCASE(idis_seq_timepoint_difference)
{
	idis::seq::timepoint a{idis::seq::tick{5}};
	idis::seq::timepoint b{idis::seq::tick{2}};
	auto delta = a - b;
	EXPECT_EQ(delta, idis::seq::tick{3});
}

TESTCASE(idis_seq_timepoint_seconds_between)
{
	idis::seq::timepoint a{idis::seq::tick{45}};
	idis::seq::timepoint b{idis::seq::tick{60}};
	auto delta = seconds_between(a, b);
	EXPECT_EQ(delta.count(), 0.25);
}

TESTCASE(idis_seq_timepoint_seconds_since_start)
{
	idis::seq::timepoint a{idis::seq::tick{45}};
	auto delta = seconds_since_start(a);
	EXPECT_EQ(delta.count(), 0.75);
}

TESTCASE(idis_seq_timepoint_to_string)
{
	idis::seq::timepoint a{idis::seq::tick{123}};
	auto const str = to_string(a);
	EXPECT_EQ(str, "2:03");
}
