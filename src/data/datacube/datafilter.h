#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <cstdint>
#include <functional>

namespace Vizzu::Data
{
struct RowWrapper;

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

	[[nodiscard]] bool operator()(const RowWrapper &row) const
	{
		return !function || function(row);
	}

	[[nodiscard]] bool operator==(const Filter &other) const
	{
		return hash == other.hash;
	}

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		return hash == other.hash || !function ? other
		     : !other.function
		         ? *this
		         : Filter(
		             [this_ = *this, other](const RowWrapper &row)
		             {
			             return this_(row) && other(row);
		             },
		             hash ^ other.hash);
	}

private:
	Function function;
	uint64_t hash{};
};

}

#endif
