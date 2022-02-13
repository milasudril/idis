#ifndef IDIS_SYS_ERRORCODE_HPP
#define IDIS_SYS_ERRORCODE_HPP

#include <errno.h>
#include <cstring>

namespace idis::sys
{
	class error_code
	{
	public:
		using rep = decltype(errno);

		explicit error_code(rep val): m_value{val} {}

		rep value() const { return m_value; }

	private:
		rep m_value;
	};

	inline char const* to_string(error_code val) { return strerror(val.value()); }
}

#endif