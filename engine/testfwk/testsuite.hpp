//@	{"dependencies_extra":[{"ref":"./testsuite.o", "rel":"implementation"}]}

#ifndef TESTFWK_TESTSUITE_HPP
#define TESTFWK_TESTSUITE_HPP

#include "./testcase.hpp"

#include <memory>
#include <type_traits>

namespace TestFwk::detail
{
	void registerTestcase(std::unique_ptr<Testcase>);

	template<class T>
	struct Empty
	{
		using type = T;
	};

	class TestcaseCreator
	{
	public:
		template<class T>
		TestcaseCreator(char const* name, char const* file, int line, Empty<T>)
		{
			registerTestcase(std::make_unique<T>(name, file, line));
		}
	};
}

namespace TestFwk
{
	extern Testcase* currentTestcase;
}

#define TESTCASE(name)                                                                             \
	namespace Testcases                                                                            \
	{                                                                                              \
		class name: public TestFwk::Testcase                                                       \
		{                                                                                          \
		public:                                                                                    \
			using Testcase::Testcase;                                                              \
			void doRun() override;                                                                 \
		};                                                                                         \
		namespace                                                                                  \
		{                                                                                          \
			TestFwk::detail::TestcaseCreator name##dummy{                                          \
			    #name, __FILE__, __LINE__, TestFwk::detail::Empty<name>{}};                        \
		}                                                                                          \
	}                                                                                              \
	void Testcases::name::doRun()

#endif