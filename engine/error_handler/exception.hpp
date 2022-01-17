#ifndef IDIS_EXCEPTION_HPP
#define IDIS_EXCEPTION_HPP

#include <string_view>
#include <stdexcept>

namespace idis
{
	class exception : public std::runtime_error
	{
	public:
		explicit exception(std::string_view trigger, std::string_view cause):
		std::runtime_error{std::string{trigger}.append(": ").append(cause)}
		{}
	};
}

#endif