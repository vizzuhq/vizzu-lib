#ifndef BUCKETS_H
#define BUCKETS_H

#include <ranges>
#include <span>
#include <vector>

namespace Vizzu
{

namespace Data
{
struct MarkerId;
}

namespace Gen
{
class Marker;

struct Buckets
{
	using MarkerIDGet = Data::MarkerId Marker::*;
	std::vector<Marker *> markers;
	MarkerIDGet marker_id_get{};

	explicit Buckets(std::span<Marker> markers);

	Buckets &sort(MarkerIDGet id_get);

	[[nodiscard]] bool empty() const { return markers.empty(); }

	struct const_iterator
	{
		std::span<Marker *const> data;
		const Buckets *parent;

		[[nodiscard]] auto operator*() const
		{
			return std::ranges::views::transform(data,
			    [this](Marker *marker)
			        -> std::pair<Marker &, const Data::MarkerId &>
			    {
				    return {*marker, marker->*parent->marker_id_get};
			    });
		}

		[[nodiscard]] bool operator!=(const const_iterator &oth) const
		{
			return data.data() != oth.data.data();
		}

		const_iterator &operator++();
	};

	[[nodiscard]] const_iterator begin() const
	{
		return ++const_iterator{{markers.data(), markers.data()},
		    this};
	}

	[[nodiscard]] const_iterator end() const
	{
		const auto end{markers.data() + markers.size()};
		return {{end, end}, this};
	}
};

}
}

#endif // BUCKETS_H
