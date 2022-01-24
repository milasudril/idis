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
	idis::seq::timepoint start{1};
	EXPECT_EQ(start.value(), 1);
	start.step();
	EXPECT_EQ(start, (idis::seq::timepoint{2}));
}

TESTCASE(idis_seq_timepoint_add_offset)
{
	idis::seq::timepoint start{1};
	EXPECT_EQ(start.value(), 1);
	EXPECT_EQ(start + 50, (idis::seq::timepoint{51}));
}

TESTCASE(idis_seq_timepoint_subtract_offset)
{
	idis::seq::timepoint start{1};
	EXPECT_EQ(start.value(), 1);
	EXPECT_EQ(start - 50, (idis::seq::timepoint{-49}));
}

TESTCASE(idis_seq_timepoint_difference)
{
	idis::seq::timepoint a{5};
	idis::seq::timepoint b{2};
	auto delta = a - b;
	EXPECT_EQ(delta, 3);
}
