//@	{"target":{"name":"exception.test"}}

#include "./exception.hpp"

#include "testfwk/testfwk.hpp"

#include <cstring>

static_assert(std::is_base_of_v<std::runtime_error, idis::exception>);

TESTCASE(idis_exception_create)
{
	idis::exception e{"foo", "bar"};
	auto const what = e.what();
	EXPECT_EQ(strcmp(what, "Failed to foo: bar"), 0)
}