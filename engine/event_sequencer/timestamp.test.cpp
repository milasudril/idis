//@{"target":{"name":"timestamp.test"}}

#include "./timestamp.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

TESTCASE(idis_seq_timestamp_create)
{
	idis::seq::timestamp t{idis::seq::timepoint{123},
	                       idis::seq::event_index{idis::seq::timestamp::events_per_frame}};

	EXPECT_EQ(t.frame(), (idis::seq::timepoint{123}));
	EXPECT_EQ(t.index(), (idis::seq::event_index{0xffffff}));
	auto str = to_string(t);
	EXPECT_EQ(str, "123:16777215");
}