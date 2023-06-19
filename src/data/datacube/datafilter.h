#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <cstdint>
#include <functional>

#include "data/table/datatable.h"

namespace Vizzu
{
namespace Data
{

class Filter
{
public:
	typedef std::function<bool(const RowWrapper &)> Function;

	Filter() : function(), hash(0) {}
	template <class Fn>
	Filter(Fn &&function, uint64_t hash) :
	    function(std::forward<Fn>(function)),
	    hash(hash)
	{}

	bool match(const RowWrapper &row) const
	{
		return !function || function(row);
	}

	bool operator==(const Filter &other) const
	{
		return hash == other.hash;
		// return (!function && !other.function) || hash == 123456789;
	}

private:
	Function function;
	uint64_t hash;
};

}
}

#endif
