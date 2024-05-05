#ifndef PLOTPTR_H
#define PLOTPTR_H

#include <memory>
#include <span>

namespace Vizzu::Gen
{
class Plot;
using PlotPtr = std::shared_ptr<Plot>;

class Marker;
struct Buckets
{
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
	using MarkerPtrArr = Marker *[];
	std::unique_ptr<MarkerPtrArr> markers;
	std::size_t k{};
	std::size_t n{};

	void resize(std::size_t i, std::size_t j)
	{
		if (k * n < i * j)
			markers =
			    std::make_unique_for_overwrite<MarkerPtrArr>(i * j);
		k = i;
		n = j;
	}

	void clear() { markers.reset(); }

	[[nodiscard]] std::size_t inner_size() const { return n; }

	[[nodiscard]] bool empty() const { return !markers; }

	[[nodiscard]] std::size_t size() const { return k; }

	std::span<Marker *> operator[](std::size_t ix) const
	{
		return {markers.get() + ix * n, n};
	}

	struct const_iterator
	{
		std::span<Marker *const> data;

		[[nodiscard]] const std::span<Marker *const> &
		operator*() const
		{
			return data;
		}

		[[nodiscard]] bool operator!=(const const_iterator &oth) const
		{
			return data.data() != oth.data.data();
		}

		const_iterator &operator++()
		{
			data = {data.data() + data.size(), data.size()};
			return *this;
		}
	};

	[[nodiscard]] const_iterator begin() const
	{
		return {operator[](0)};
	}

	[[nodiscard]] const_iterator end() const
	{
		return {operator[](k)};
	}
};
}

#endif
