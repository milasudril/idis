//@	{"target":{"name":"sorted_sequence.test"}}

#include "./sorted_sequence.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_helpers_sorted_sequence_create)
{
	idis::sorted_sequence<int> seq_a{1, 3, 2, 4};
	EXPECT_EQ(seq_a[0], 1);
	EXPECT_EQ(seq_a[1], 2);
	EXPECT_EQ(seq_a[2], 3);
	EXPECT_EQ(seq_a[3], 4);
	EXPECT_EQ(std::size(seq_a), 4);

	idis::sorted_sequence<int> seq_b{4, 2, 3, 1};
 	EXPECT_EQ(seq_a == seq_b, true);

	auto seq_c{seq_a};
	EXPECT_EQ(seq_c != seq_a, false);
}

TESTCASE(idis_helpers_sorted_sequence_create_view)
{
	idis::sorted_sequence<int> seq_a{1, 3, 2, 4};
	auto view = idis::sorted_view{std::ref(seq_a)};

	EXPECT_EQ(view[0], 1);
	EXPECT_EQ(view[1], 2);
	EXPECT_EQ(view[2], 3);
	EXPECT_EQ(view[3], 4);
}