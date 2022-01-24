//@{"target":{"name":"timepoint.test"}}

#include "./timepoint.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

TESTCASE(idis_seq_tick_to_string)
{
	auto const time_difference = idis::seq::tick{-70};
	printf("%s\n", to_string(time_difference).c_str());
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
