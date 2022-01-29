//@{"target":{"name":"timestamp.test"}}

#include "./timestamp.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_seq_timestamp_create)
{
	idis::seq::timestamp t{idis::seq::timepoint{idis::seq::tick{123}}, idis::seq::event_index{456}};

	EXPECT_EQ(t.frame(), idis::seq::timepoint{idis::seq::tick{123}});
	EXPECT_EQ(t.index(), idis::seq::event_index{456});
	auto str = to_string(t);
	EXPECT_EQ(str, "2:03 (456)");
}
