
#ifndef DATAFRAME_OLD_TYPES_H
#define DATAFRAME_OLD_TYPES_H

#include <base/type/uniquelist.h>
#include <set>

#include "../interface.h"

namespace Vizzu::Data
{

class DataTable;
class DataCube;

struct RowWrapper
{
	[[nodiscard]] dataframe::cell_value get_value(
	    const std::string_view &i) const
	{
		return parent->get_data(recordId, i);
	}

	const dataframe::dataframe_interface *parent;
	std::size_t recordId;
};

class SeriesIndex
{
	std::string orig_name;
	std::string_view sid;
	std::optional<dataframe::aggregator_type> aggr;

public:
	SeriesIndex(std::string const &str, const DataTable &table);

	[[nodiscard]] const dataframe::aggregator_type &getAggr() const
	{
		return *aggr;
	}

	[[nodiscard]] const std::string_view &getColIndex() const;

	bool operator==(const SeriesIndex &rhs) const;
	bool operator<(const SeriesIndex &rhs) const;

	[[nodiscard]] bool isDimension() const;

	[[nodiscard]] const std::string &toString() const
	{
		return orig_name;
	}
};

using SeriesList = Type::UniqueList<SeriesIndex>;

class DataCubeOptions
{
public:
	using IndexSet = std::set<SeriesIndex>;

	DataCubeOptions(IndexSet &&dims, IndexSet &&msrs) :
	    dimensions(std::move(dims)),
	    measures(std::move(msrs))
	{}

	[[nodiscard]] const IndexSet &getDimensions() const
	{
		return dimensions;
	}

	[[nodiscard]] const IndexSet &getMeasures() const
	{
		return measures;
	}

private:
	IndexSet dimensions;
	IndexSet measures;
};

class Filter
{
public:
	using Function = std::function<bool(const RowWrapper &)>;

	Filter() noexcept = default;

	template <class Ptr, class Deleter>
	explicit Filter(std::unique_ptr<Ptr, Deleter> &&wr) :
	    hash(std::hash<bool (*)(const RowWrapper *)>{}(wr.get())),
	    function(
	        [wr = std::shared_ptr{std::move(wr)}](
	            const RowWrapper &row) noexcept -> bool
	        {
		        return (*wr)(&row);
	        })
	{}

	template <class Fun>
	Filter(std::size_t hash, Fun &&function) :
	    hash(hash),
	    function(std::forward<Fun>(function))
	{}

	[[nodiscard]] bool operator==(const Filter &other) const
	{
		return hash == other.hash;
	}

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		return {hash == other.hash ? hash : hash ^ other.hash,
		    [fun1 = this->function, fun2 = other.function](
		        const RowWrapper &row) noexcept -> bool
		    {
			    return (!fun1 || fun1(row)) && (!fun2 || fun2(row));
		    }};
	}

	[[nodiscard]] const Function &getFunction() const
	{
		return function;
	}

private:
	std::size_t hash{};
	Function function;
};

struct SliceIndex
{
	std::string_view column;
	std::string_view value;

	[[nodiscard]] bool operator<(const SliceIndex &) const;

	[[nodiscard]] bool operator==(const SliceIndex &) const = default;
};

using SubSliceIndex = std::vector<SliceIndex>;

struct CellInfo
{
	std::vector<std::pair<std::string, std::string>> categories;
	std::vector<std::pair<std::string, double>> values;
};

struct MultiIndex
{
	std::optional<std::size_t> rid;
	std::vector<std::size_t> old;

	[[nodiscard]] bool isEmpty() const;
};

struct MarkerId
{
	SubSliceIndex itemSliceIndex;
	std::size_t seriesId{};
	std::size_t itemId{};

	[[nodiscard]] bool operator==(const MarkerId &) const;
};
}

#endif