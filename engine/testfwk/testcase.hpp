#ifndef TESTFWK_TESTCASE_HPP
#define TESTFWK_TESTCASE_HPP

#include <cstdio>
#include <stdexcept>

namespace TestFwk
{
	class Testcase
	{
	public:
		explicit Testcase(char const* name, char const* file, int line)
		    : m_name{name}
		    , m_file{file}
		    , m_line{line}
		    , m_failed{false}
		{
		}

		virtual ~Testcase() = default;

		void testcaseFailed() { m_failed = true; }

		bool failed() const { return m_failed; }

		bool run()
		{
			try
			{
				doRun();
			}
			catch(std::exception const& e)
			{
				fprintf(stderr, "%s:%d: error: %s\n", m_file, m_line, e.what());
				testcaseFailed();
			}
			catch(...)
			{
				fprintf(stderr, "%s:%d: error: %s\n", m_file, m_line, "Exception thrown");
				testcaseFailed();
			}
			if(m_failed) { fprintf(stderr, "%s:%d: error: Testcase failed\n", m_file, m_line); }
			printf("%s %s (%s:%d)\n", m_failed ? "✗" : "✓", m_name, m_file, m_line);
			return !m_failed;
		}

	private:
		virtual void doRun() = 0;
		char const* m_name;
		char const* m_file;
		int m_line;
		bool m_failed;
	};
}
#endif