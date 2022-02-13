#ifndef IDIS_UTILS_SORTEDSEQUENCE_HPP
#define IDIS_UTILS_SORTEDSEQUENCE_HPP

#include <algorithm>

namespace idis
{
	template<class T,
	         class Container = std::vector<T>,
	         class Compare   = std::less<typename Container::value_type>>
	class sorted_sequence
	{
	public:
		using container_type = Container;
		using value_compare  = Compare;
		using value_type     = Container::value_type;
		using size_type      = Container::size_type;
		using const_iterator = Container::const_iterator;

		sorted_sequence() = default;

		template<class First, class... Args>
		requires(!std::is_same_v<std::decay_t<First>, sorted_sequence>) explicit sorted_sequence(
		    First&& first, Args&&... args)
		    : m_container{std::forward<First>(first), std::forward<Args>(args)...}
		{
			std::ranges::sort(m_container, Compare{});
		}

		decltype(auto) begin() const { return std::begin(m_container); }

		decltype(auto) end() const { return std::end(m_container); }

		decltype(auto) size() const { return std::size(m_container); }

		decltype(auto) data() const { return std::data(m_container); }

		decltype(auto) operator[](size_t index) const { return m_container[index]; }

		auto operator<=>(sorted_sequence const&) const = default;

	private:
		Container m_container;
	};

	template<class T, class Compare = std::less<T>>
	class sorted_view
	{
	public:
		using value_compare  = Compare;
		using value_type     = T;
		using size_type      = size_t;
		using const_iterator = T const*;

		template<class Container>
		sorted_view(std::reference_wrapper<sorted_sequence<T, Container, Compare> const> src)
		    : m_begin{std::data(src.get())}
		    , m_size{std::size(src.get())}
		{
		}

		template<class Container>
		sorted_view(sorted_sequence<T, Container, Compare>& src)
		    : m_begin{std::data(src)}
		    , m_size{std::size(src)}
		{
		}

		auto begin() const { return m_begin; }

		auto end() const { return m_begin + m_size; }

		auto size() const { return m_size; }

		auto data() const { return m_begin; }

		auto const& operator[](size_t index) const { return m_begin[index]; }

	private:
		T const* m_begin;
		size_t m_size;
	};
}

#endif