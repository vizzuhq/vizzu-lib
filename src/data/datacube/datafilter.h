#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <cstdint>
#include <functional>

#include "data/table/datatable.h"

namespace Vizzu::Data
{

class Filter
{
public:
	using Function = std::function<bool(const RowWrapper &)>;

	Filter() = default;
	template <class Fn>
	Filter(Fn &&function, uint64_t hash) :
	    function(std::forward<Fn>(function)),
	    hash(hash)
	{}

	[[nodiscard]] bool match(const RowWrapper &row) const
	{
		return !function || function(row);
	}

	bool operator==(const Filter &other) const
	{
		return hash == other.hash;
	}

private:
	Function function;
	uint64_t hash {};
};

}

#endif
