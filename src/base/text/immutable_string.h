#ifndef IMMUTABLE_STRING_H
#define IMMUTABLE_STRING_H

#include <cstring>
#include <memory>
#include <new>
#include <string_view>
#include <utility>

namespace Text
{

struct immutable_string
{
	struct impl_t
	{
		std::size_t counter;
		const std::size_t size;
	};

	struct deleter_t
	{
		void operator()(char *buffer) const noexcept
		{
			::operator delete[](buffer,
			    std::align_val_t{alignof(impl_t)});
		}

		void operator()(impl_t *impl) const noexcept
		{
			auto *buffer = std::launder<char>(
			    static_cast<char *>(static_cast<void *>(impl)));
			impl->~impl_t();
			::operator delete(buffer, impl);
			(*this)(buffer);
		}
	};

	struct allocator_t
	{
		impl_t *operator()(const char *data, std::size_t length) const
		{
			std::unique_ptr<char, deleter_t> ptr{
			    new (std::align_val_t{alignof(
			        impl_t)}) char[sizeof(impl_t) + length + 1]};
			std::memcpy(ptr.get() + sizeof(impl_t), data, length);
			ptr.get()[sizeof(impl_t) + length] = '\0';

			static_assert(std::is_nothrow_constructible_v<impl_t,
			    std::size_t,
			    std::size_t>);
			auto res = std::unique_ptr<impl_t, deleter_t>{
			    new (ptr.release()) impl_t{1, length}};
			return res.release();
		}
	};

	[[nodiscard]] constexpr immutable_string() noexcept : impl{} {}

	[[nodiscard]] explicit constexpr immutable_string(
	    std::nullptr_t) noexcept :
	    impl{}
	{}

	[[nodiscard]] explicit immutable_string(
	    const std::string_view &data) :
	    impl(allocator_t{}(data.data(), data.size()))
	{}

	template <std::size_t N>
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
	using CCharArr = const char[N];

	template <std::size_t N>
	// NOLINTNEXTLINE(google-explicit-constructor)
	[[nodiscard]] immutable_string(CCharArr<N> &data) :
	    immutable_string(std::string_view{data, N - 1})
	{}

	template <std::size_t N>
	// NOLINTNEXTLINE(google-explicit-constructor)
	[[nodiscard]] immutable_string(CCharArr<N> &&data) :
	    immutable_string(std::string_view{data, N - 1})
	{}

	[[nodiscard]] immutable_string(
	    const immutable_string &other) noexcept :
	    impl(other.impl)
	{
		if (impl) ++impl->counter;
	}

	[[nodiscard]] immutable_string(immutable_string &&other) noexcept
	    :
	    impl(other.impl)
	{
		other.impl = nullptr;
	}

	immutable_string &operator=(
	    const immutable_string &other) noexcept
	{
		if (this == &other) return *this;
		decr();
		impl = other.impl;
		if (impl) ++impl->counter;
		return *this;
	}

	immutable_string &operator=(immutable_string &&other) noexcept
	{
		if (this == &other) return *this;
		decr();
		impl = other.impl;
		other.impl = nullptr;
		return *this;
	}

	immutable_string &operator=(const char *data)
	{
		decr();
		impl = allocator_t{}(data, std::strlen(data));
		return *this;
	}

	~immutable_string() noexcept { decr(); }

	void decr()
	{
		if (impl && --impl->counter == 0)
			deleter_t{}(std::exchange(impl, nullptr));
	}

	[[nodiscard]] const char *c_str() const noexcept
	{
		if (!impl) return "";
		return view().data();
	}

	// NOLINTNEXTLINE(google-explicit-constructor)
	[[nodiscard]] operator std::string_view() const noexcept
	{
		if (!impl) return {};
		return {std::launder<const char>(static_cast<const char *>(
		            static_cast<const void *>(impl)))
		            + sizeof(impl_t),
		    impl->size};
	}

	[[nodiscard]] std::string_view view() const noexcept
	{
		return *this;
	}

	[[nodiscard]] bool empty() const noexcept
	{
		return !impl || impl->size == 0;
	}

	[[nodiscard]] bool operator==(
	    const immutable_string &other) const noexcept
	{
		return view() == other.view();
	}

	[[nodiscard]] auto operator<=>(
	    const immutable_string &other) const noexcept
	{
		return view() <=> other.view();
	}

	[[nodiscard]] friend bool operator==(const immutable_string &lhs,
	    const std::string_view &rhs) noexcept
	{
		return lhs.view() == rhs;
	}

	[[nodiscard]] friend bool operator==(const std::string_view &lhs,
	    const immutable_string &rhs) noexcept
	{
		return lhs == rhs.view();
	}

	template <std::size_t N>
	[[nodiscard]] friend bool operator==(const immutable_string &lhs,
	    CCharArr<N> &rhs) noexcept
	{
		return lhs == std::string_view{rhs, N - 1};
	}

	template <std::size_t N>
	[[nodiscard]] friend bool operator==(CCharArr<N> &lhs,
	    const immutable_string &rhs) noexcept
	{
		return std::string_view{lhs, N - 1} == rhs;
	}

	template <std::size_t N>
	[[nodiscard]] friend bool operator==(const immutable_string &lhs,
	    CCharArr<N> &&rhs) noexcept
	{
		return lhs == std::string_view{rhs, N - 1};
	}

	template <std::size_t N>
	[[nodiscard]] friend bool operator==(CCharArr<N> &&lhs,
	    const immutable_string &rhs) noexcept
	{
		return std::string_view{lhs, N - 1} == rhs;
	}

	[[nodiscard]] friend auto operator<=>(const immutable_string &lhs,
	    const std::string_view &rhs) noexcept
	{
		return lhs.view() <=> rhs;
	}

	[[nodiscard]] friend auto operator<=>(const std::string_view &lhs,
	    const immutable_string &rhs) noexcept
	{
		return lhs <=> rhs.view();
	}

	template <std::size_t N>
	[[nodiscard]] friend auto operator<=>(const immutable_string &lhs,
	    CCharArr<N> &rhs) noexcept
	{
		return lhs <=> std::string_view{rhs, N - 1};
	}

	template <std::size_t N>
	[[nodiscard]] friend auto operator<=>(CCharArr<N> &lhs,
	    const immutable_string &rhs) noexcept
	{
		return std::string_view{lhs, N - 1} <=> rhs;
	}

	template <std::size_t N>
	[[nodiscard]] friend auto operator<=>(const immutable_string &lhs,
	    CCharArr<N> &&rhs) noexcept
	{
		return lhs <=> std::string_view{rhs, N - 1};
	}

	template <std::size_t N>
	[[nodiscard]] friend auto operator<=>(CCharArr<N> &&lhs,
	    const immutable_string &rhs) noexcept
	{
		return std::string_view{lhs, N - 1} <=> rhs;
	}

	friend std::string operator+(const immutable_string &lhs,
	    const std::string &rhs)
	{
		return lhs.toString() + rhs;
	}

	friend std::string operator+(const std::string &lhs,
	    const immutable_string &rhs)
	{
		return lhs + rhs.toString();
	}

	friend std::string operator+(const immutable_string &lhs,
	    std::string &&rhs)
	{
		return lhs.toString() + std::move(rhs);
	}

	friend std::string operator+(std::string &&lhs,
	    const immutable_string &rhs)
	{
		return std::move(lhs) + rhs.toString();
	}

	[[nodiscard]] std::string toString() const
	{
		return std::string{view()};
	}

	[[nodiscard]] static immutable_string fromString(
	    const std::string &data)
	{
		return immutable_string{data};
	}

	impl_t *impl;
};

}

#endif // IMMUTABLE_STRING_H
