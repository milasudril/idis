#ifndef TESTFWK_VALIDATION_HPP
#define TESTFWK_VALIDATION_HPP

#include "./testsuite.hpp"

#include <cstdio>
#include <string>
#include <filesystem>
#include <vector>

namespace TestFwk::detail
{
	inline std::string const& format(std::string const& str) { return str; }

	inline std::string const& format(std::string const&&) = delete;

	template<class T, size_t N>
	std::string to_string(std::array<T, N> const& vals);

	inline std::string to_string(std::byte val)
	{
		return std::to_string(static_cast<uint32_t>(val));
	}

	inline std::string to_string(std::filesystem::path const& path) { return path.string(); }

	template<class T, class U>
	std::string to_string(std::pair<T, U> const& x)
	{
		using std::to_string;
		using TestFwk::detail::to_string;
		std::string ret{"("};
		ret += to_string(x.first);
		ret += ", ";
		ret += to_string(x.second);
		ret += ")";
		return ret;
	}

	template<class T, size_t N>
	std::string to_string(std::array<T, N> const& vals)
	{
		using std::to_string;
		using TestFwk::detail::to_string;
		std::string ret{"["};
		for(size_t k = 0; k < N; ++k)
		{
			ret += to_string(vals[k]);
			if(k != N - 1) { ret += ", "; }
		}
		ret += "]";
		return ret;
	}

	template<class T>
	std::string to_string(std::vector<T> const& vals)
	{
		using std::to_string;
		using TestFwk::detail::to_string;
		auto const n = std::size(vals);
		std::string ret{"["};
		for(size_t k = 0; k < n; ++k)
		{
			ret += to_string(vals[k]);
			if(k != n - 1) { ret += ", "; }
		}
		ret += "]";
		return ret;
	}


	inline std::string to_string(void const* val)
	{
		return std::to_string(reinterpret_cast<intptr_t>(val));
	}

	template<class T>
	std::string format(T const& value)
	{
		using std::to_string;
		using TestFwk::detail::to_string;
		return to_string(value);
	}

}

#define TESTFWK_STRINGIFY(x) #x
#define TESTFWK_TOSTRING(x) TESTFWK_STRINGIFY(x)

#define CHECK_OP(a, op, b, type)                                                                   \
	if(!((a)op(b)))                                                                                \
	{                                                                                              \
		fprintf(stderr,                                                                            \
		        "%s%s%s%s\n",                                                                      \
		        __FILE__ ":" TESTFWK_TOSTRING(__LINE__) ": error: " type " " #a " " #op " " #b     \
		                                                ", but " #a " == ",                        \
		        TestFwk::detail::format(a).c_str(),                                                \
		        ", and " #b " == ",                                                                \
		        TestFwk::detail::format(b).c_str());                                               \
		TestFwk::currentTestcase->testcaseFailed();                                                \
	}

#define EXPECT_OP(a, op, b) CHECK_OP(a, op, b, "Expected")

#define EXPECT_EQ(a, b) EXPECT_OP(a, ==, b)
#define EXPECT_LT(a, b) EXPECT_OP(a, <, b)
#define EXPECT_GT(a, b) EXPECT_OP(a, >, b)
#define EXPECT_NE(a, b) EXPECT_OP(a, !=, b)
#define EXPECT_GE(a, b) EXPECT_OP(a, >=, b)
#define EXPECT_LE(a, b) EXPECT_OP(a, <=, b)

#define REQUIRE_OP(a, op, b)                                                                       \
	CHECK_OP(a, op, b, "Required")                                                                 \
	if(!((a)op(b))) { throw std::runtime_error{"Requirement not satisfied. Testcase aborted."}; }

#define REQUIRE_EQ(a, b) REQUIRE_OP(a, ==, b)
#define REQUIRE_LT(a, b) REQUIRE_OP(a, <, b)
#define REQUIRE_GT(a, b) REQUIRE_OP(a, >, b)
#define REQUIRE_NE(a, b) REQUIRE_OP(a, !=, b)
#define REQUIRE_GE(a, b) REQUIRE_OP(a, >=, b)
#define REQUIRE_LE(a, b) REQUIRE_OP(a, <=, b)

#endif