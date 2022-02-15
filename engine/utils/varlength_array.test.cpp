//@	{"target":{"name":"varlength_array.test"}}

#include "./varlength_array.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(idis_utils_varlength_array_default_state)
{
	idis::varlength_array<int> foo{};

	EXPECT_EQ(std::begin(foo), nullptr);
	EXPECT_EQ(std::end(foo), std::begin(foo));
	EXPECT_EQ(std::size(foo), 0);
	EXPECT_EQ(std::data(foo), nullptr);
	EXPECT_EQ(foo.empty(), true);
}

TESTCASE(idis_utils_varlength_array_construct_from_value_and_do_stuff)
{
	idis::varlength_array<std::string> foo{10, "A string to long for sbo"};

	EXPECT_EQ(std::size(foo), 10);
	EXPECT_NE(std::begin(foo), nullptr);
	EXPECT_EQ(std::end(foo), std::begin(foo) + 10);
	EXPECT_EQ(std::data(foo), std::begin(foo));
	EXPECT_EQ(foo.empty(), false);

	REQUIRE_GE(std::size(foo), 10);
	for(size_t k = 0; k != 10; ++k)
	{
		EXPECT_EQ(foo[k], "A string to long for sbo");
	}

	auto bar = foo;
	EXPECT_NE(std::begin(foo), std::begin(bar));
	EXPECT_NE(std::end(foo), std::end(bar));
	EXPECT_NE(std::data(foo), std::data(bar));
	EXPECT_EQ(std::size(foo), std::size(bar));

	REQUIRE_EQ(std::size(bar), std::size(foo));
	for(size_t k = 0; k != 10; ++k)
	{
		EXPECT_EQ(bar[k], foo[k]);
	}

	foo = bar;

	auto orig_data = std::data(foo);

	auto other = std::move(foo);
	EXPECT_EQ(std::data(other), orig_data);
	EXPECT_EQ(foo.empty(), true);
	EXPECT_EQ(other.empty(), false);
	EXPECT_EQ(bar.empty(), false);

	bar = std::move(other);
	EXPECT_EQ(other.empty(), true);
	EXPECT_EQ(bar.empty(), false);
}


namespace
{
	struct type_with_throwing_ctor
	{
		int k;

		type_with_throwing_ctor() { ++instance_counter; }

		type_with_throwing_ctor(type_with_throwing_ctor const&)
		{
			if(instance_counter > 5) { throw std::string{"This is bad"}; }
			k = instance_counter;
			++instance_counter;
		}

		~type_with_throwing_ctor() { --instance_counter; }

		static int instance_counter;
	};

	constinit int type_with_throwing_ctor::instance_counter = 0;
}

TESTCASE(idis_utils_varlength_array_construct_with_exception)
{
	try
	{
		idis::varlength_array<type_with_throwing_ctor> foo{10};
	}
	catch(std::string const& val)
	{
		EXPECT_EQ(val, "This is bad");
	}
	EXPECT_EQ(type_with_throwing_ctor::instance_counter, 0);
}

namespace
{
	struct holder
	{
		explicit holder(int val): m_val{std::make_unique<int>(val)} {}

		std::unique_ptr<int> m_val;
	};
}

TESTCASE(idis_utils_varlength_array_construct_from_range)
{
	std::array<int, 4> values{1, 2, 3, 4};

	idis::varlength_array<int> foo{values};

	auto other = idis::varlength_array<holder>{foo};
}