//@	{"target":{"name":"testsuite.o"}}

#include "./testsuite.hpp"

#include <algorithm>
#include <vector>

namespace
{
	std::vector<std::unique_ptr<TestFwk::Testcase>>& testcases()
	{
		static std::vector<std::unique_ptr<TestFwk::Testcase>> tcs;
		return tcs;
	}
}

void TestFwk::detail::registerTestcase(std::unique_ptr<TestFwk::Testcase> tc)
{
	testcases().push_back(std::move(tc));
}

TestFwk::Testcase* TestFwk::currentTestcase;

int main()
{
	if(testcases().size() == 0) { fprintf(stderr, "Warning: Testsuite has no testcases\n"); }

	size_t n = std::count_if(std::begin(testcases()),
	                         std::end(testcases()),
	                         [](auto const& item)
	                         {
		                         TestFwk::currentTestcase = item.get();
		                         return item->run();
	                         });

	if(n != testcases().size())
	{
		fprintf(stderr,
		        "Error: %zu of %zu testcases failed\n",
		        testcases().size() - n,
		        testcases().size());
	}

	return !(n == testcases().size());
}