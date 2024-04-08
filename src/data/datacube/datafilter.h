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
	Filter(Fn &&function, uint64_t hashVal) :
	    function(std::forward<Fn>(function)),
	    hash(hashVal)
	{}

	[[nodiscard]] bool match(const RowWrapper &row) const
	{
		return !function || function(row);
	}

	[[nodiscard]] bool operator==(const Filter &other) const
	{
		return hash == other.hash;
	}

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		return get_hash() == other.get_hash() || !has() ? other
		     : !other.has()
		         ? *this
		         : Data::Filter(
		             [this_ = *this, other](
		                 const Data::RowWrapper &row)
		             {
			             return this_.match(row) && other.match(row);
		             },
		             get_hash() ^ other.get_hash());
	}

private:
	[[nodiscard]] bool has() const
	{
		return static_cast<bool>(function);
	}

	[[nodiscard]] uint64_t get_hash() const { return hash; }

	Function function;
	uint64_t hash{};
};

}

#endif
