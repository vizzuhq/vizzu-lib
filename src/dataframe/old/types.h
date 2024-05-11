
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

	[[nodiscard]] bool operator==(const SeriesIndex &rhs) const;
	[[nodiscard]] bool operator<(const SeriesIndex &rhs) const;

	friend bool operator<(const std::string_view &dim,
	    const SeriesIndex &rhs);
	friend bool operator<(const SeriesIndex &lhs,
	    const std::string_view &dim);

	[[nodiscard]] bool isDimension() const;

	[[nodiscard]] const std::string &toString() const
	{
		return orig_name;
	}
};

using SeriesList = Type::UniqueList<SeriesIndex>;

class Filter
{
	using Fun = bool(const RowWrapper *);
	using SharedFun = std::shared_ptr<Fun>;
	constexpr static Fun *True = +[](const RowWrapper *)
	{
		return true;
	};

	Filter(Fun *lhs, Fun *rhs) noexcept :
	    func1{std::shared_ptr<void>{}, lhs},
	    func2{std::shared_ptr<void>{}, rhs}
	{}

public:
	Filter() noexcept = default;

	template <class Pointer>
	explicit Filter(Pointer &&wr) : func1{std::forward<Pointer>(wr)}
	{}

	[[nodiscard]] bool operator==(
	    const Filter &other) const = default;

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		return {func1.get(),
		    func1 == other.func1 ? True : other.func1.get()};
	}

	[[nodiscard]] auto getFunction() const
	{
		return [this](const RowWrapper &row)
		{
			return (*func1)(&row) && (*func2)(&row);
		};
	}

private:
	SharedFun func1{std::shared_ptr<void>{}, True};
	SharedFun func2{std::shared_ptr<void>{}, True};
};

struct SliceIndex
{
	std::string_view column;
	std::string_view value;

	[[nodiscard]] bool operator<(const SliceIndex &) const;

	[[nodiscard]] bool operator==(const SliceIndex &) const = default;
};

struct CellInfo
{
	std::vector<std::pair<std::string, std::string>> markerInfo;

	std::string json;
};

struct MultiIndex
{
	std::optional<std::size_t> rid;
	std::vector<std::size_t> old;

	[[nodiscard]] bool isEmpty() const;
};

struct MarkerId
{
	std::optional<SliceIndex> label;
	std::size_t seriesId{};
	std::size_t itemId{};

	[[nodiscard]] bool operator==(const MarkerId &) const;
};
}

#endif