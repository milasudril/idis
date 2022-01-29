#ifndef IDIS_HELPERS_FD_HPP
#define IDIS_HELPERS_FD_HPP

#include <cstddef>
#include <unistd.h>

namespace idis
{
	struct fd
	{
		using pointer = int;

		fd() = default;

		fd(int val): descriptor{val} {}
		fd(std::nullptr_t): descriptor{-1} {}

		operator int() const { return descriptor; }

		bool operator==(fd other) const { return descriptor == other.descriptor; }
		bool operator!=(fd other) const { return descriptor != other.descriptor; }
		bool operator==(std::nullptr_t) const { return descriptor == -1; }
		bool operator!=(std::nullptr_t) const { return descriptor != -1; }

		int descriptor;
	};

	struct fd_deleter
	{
		using pointer = fd;
		void operator()(fd descriptor) const { close(descriptor); }
	};

	template<class T>
	void unused(T&&)
	{
	}
}

#endif