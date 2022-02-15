#ifndef IDIS_VARLENGTHARRAY_HPP
#define IDIS_VARLENGTHARRAY_HPP

#include <cstddef>
#include <memory>
#include <concepts>
#include <ranges>

namespace idis
{
	template<class T, class Func, class... Args>
	T* allocate_and(size_t n, Func&& f, Args&&... args)
	{
		auto ret = std::allocator<T>{}.allocate(n);
		try
		{
			f(ret, n, std::forward<Args>(args)...);
		}
		catch(...)
		{
			std::allocator<T>{}.deallocate(ret, n);
			throw;
		}
		return ret;
	}

	template<class T>
	T* allocate_and_fill(size_t n, T const& default_val)
	{
		return allocate_and<T>(
		    n,
		    [](T* buffer, size_t n, T const& default_val)
		    { std::uninitialized_fill_n(buffer, n, default_val); },
		    default_val);
	}

	template<class T, std::ranges::range Range>
	T* allocate_and_copy(Range&& src)
	{
		return allocate_and<T>(
		    std::size(src),
		    [](T* buffer, size_t n, auto&& src)
		    { std::uninitialized_copy_n(std::begin(src), n, buffer); },
		    src);
	}

	template<class T>
	class varlength_array
	{
	public:
		using value_type     = T;
		using size_type      = size_t;
		using const_iterator = T const*;
		using iterator       = T*;

		varlength_array(): m_data{nullptr}, m_size{0} {}

		explicit varlength_array(size_t n, T const& default_val = T{})
		    : m_data{allocate_and_fill(n, default_val)}
		    , m_size{n}
		{
		}

		template<std::ranges::range Range>
		requires(!std::is_same_v<std::decay_t<Range>, varlength_array>) explicit varlength_array(
		    Range&& src)
		    : m_data{allocate_and_copy(src)}
		    , m_size{std::size(src)}
		{
		}

		varlength_array(varlength_array const& src)
		    : m_data{allocate_and_copy<T>(src)}
		    , m_size{std::size(src)}
		{
		}

		varlength_array(varlength_array&& other) noexcept
		    : m_data{std::data(other)}
		    , m_size{std::size(other)}
		{
			other.m_data = nullptr;
			other.m_size = 0;
		}

		varlength_array& operator=(varlength_array&& other) noexcept
		{
			clear();
			m_data       = std::data(other);
			m_size       = std::size(other);
			other.m_data = nullptr;
			other.m_size = 0;
			return *this;
		}

		varlength_array& operator=(varlength_array const& other)
		{
			varlength_array tmp{other};
			*this = std::move(tmp);
			return *this;
		}

		~varlength_array() { clear(); }

		auto begin() const { return m_data; }

		auto end() const { return m_data + m_size; }

		auto data() const { return m_data; }

		auto const& operator[](size_t index) const { return m_data[index]; }

		auto begin() { return m_data; }

		auto end() { return m_data + m_size; }

		auto data() { return m_data; }

		auto& operator[](size_t index) { return m_data[index]; }

		auto size() const { return m_size; }

		void clear()
		{
			if(m_data != nullptr)
			{
				std::destroy_n(m_data, m_size);
				std::allocator<T>{}.deallocate(m_data, m_size);
				m_data = nullptr;
				m_size = 0;
			}
		}

		bool empty() const { return m_data == nullptr; }


	private:
		T* m_data;
		size_t m_size;
	};
}


#endif