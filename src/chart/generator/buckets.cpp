
#include "buckets.h"

#include <numeric>

#include "dataframe/old/types.h"

#include "marker.h"

namespace Vizzu::Gen
{

Buckets::Buckets(std::span<Marker> markers) : markers(markers.size())
{
	std::iota(this->markers.begin(),
	    this->markers.end(),
	    markers.data());
}

Buckets &Buckets::sort(MarkerIDGet id_get)
{
	marker_id_get = id_get;
	std::ranges::stable_sort(markers,
	    [id_get](Marker *lhs, Marker *rhs) -> bool
	    {
		    if (auto &&cmp = lhs->*id_get <=> rhs->*id_get;
		        std::is_neq(cmp))
			    return std::is_lt(cmp);
		    return lhs < rhs;
	    });
	return *this;
}

Buckets::const_iterator &Buckets::const_iterator::operator++()
{
	const auto *const real_end =
	    parent->markers.data() + parent->markers.size();
	const auto *const curr_end = data.data() + data.size();
	data = {curr_end,
	    curr_end == real_end
	        ? curr_end
	        : std::partition_point(curr_end,
	              real_end,
	              [this,
	                  searched = (*curr_end->*parent->marker_id_get)
	                                 .seriesId](Marker *lhs) -> bool
	              {
		              return (lhs->*parent->marker_id_get).seriesId
		                  == searched;
	              })};
	return *this;
}

}