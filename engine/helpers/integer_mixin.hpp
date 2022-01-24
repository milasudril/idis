#ifndef IDIS_HELPERS_INTEGERMIXIN_HPP
#define IDIS_HELPERS_INTEGERMIXIN_HPP

#include <type_traits>

namespace idis
{
	template<class Rep, class UserClass>
	class integer_mixin
	{
	public:
		using rep = Rep;

		constexpr explicit integer_mixin(rep val): m_value{val} {}

		constexpr auto value() const { return m_value; }

		static constexpr auto zero() { return UserClass{0}; }

		constexpr auto operator<=>(integer_mixin const& other) const = default;

		constexpr auto& operator++()
		{
			++m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto operator++(int)
		{
			auto tmp = *this;
			++m_value;
			return UserClass{tmp.value()};
		}

		constexpr auto& operator--()
		{
			--m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto operator--(int)
		{
			auto tmp = *this;
			--m_value;
			return UserClass{tmp.value()};
		}

		constexpr auto& operator+=(UserClass other)
		{
			m_value += other.m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator+=(integer_mixin other)
		{
			m_value += other.m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator-=(UserClass other)
		{
			m_value -= other.m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator-=(integer_mixin other)
		{
			m_value -= other.m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator*=(rep value)
		{
			m_value *= value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator/=(rep value)
		{
			m_value /= value;
			return static_cast<UserClass&>(*this);
		}


	private:
		rep m_value;
	};

	template<class UserClass>
	constexpr auto operator+(UserClass a, UserClass b) requires(
	    std::is_base_of_v<integer_mixin<typename UserClass::rep, UserClass>, UserClass>)
	{

		return a += b;
	}

	template<class UserClass>
	constexpr auto operator-(UserClass a, UserClass b) requires(
	    std::is_base_of_v<integer_mixin<typename UserClass::rep, UserClass>, UserClass>)
	{
		return a -= b;
	}

	template<class UserClass>
	requires(std::is_base_of_v<integer_mixin<typename UserClass::rep, UserClass>,
	                           UserClass>) constexpr auto
	operator*(typename UserClass::rep c, UserClass b)
	{
		return b *= c;
	}

	template<class UserClass>
	requires(std::is_base_of_v<integer_mixin<typename UserClass::rep, UserClass>,
	                           UserClass>) constexpr auto
	operator/(UserClass a, typename UserClass::rep c)
	{
		return a /= c;
	}
}
#endif
