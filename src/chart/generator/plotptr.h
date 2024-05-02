#ifndef PLOTPTR_H
#define PLOTPTR_H

#include <memory>
#include <span>
#include <vector>

namespace Vizzu::Gen
{
class Plot;
using PlotPtr = std::shared_ptr<Plot>;

class Marker;
struct Buckets
{
	std::vector<Marker *> markers;
	std::size_t k{};
	std::size_t n{};

	void resize(std::size_t i, std::size_t j)
	{
		markers.resize(i * j);
		k = i;
		n = j;
	}

	void clear() { markers.clear(); }

	[[nodiscard]] bool empty() const { return markers.empty(); }

	[[nodiscard]] std::size_t size() const { return k; }

	std::span<Marker *> operator[](std::size_t ix)
	{
		return {markers.data() + ix * n, n};
	}

	std::span<Marker *const> operator[](std::size_t ix) const
	{
		return {markers.data() + ix * n, n};
	}

	struct const_iterator
	{
		Marker *const *at;
		std::size_t n;
		[[nodiscard]] std::span<Marker *const> operator*() const
		{
			return {at, n};
		}

		[[nodiscard]] bool operator!=(const const_iterator &oth) const
		{
			return at != oth.at;
		}
		const_iterator &operator++()
		{
			at += n;
			return *this;
		}
	};

	[[nodiscard]] const_iterator begin() const
	{
		return {markers.data(), n};
	}

	[[nodiscard]] const_iterator end() const
	{
		return {markers.data() + markers.size(), n};
	}
};
}

#endif
