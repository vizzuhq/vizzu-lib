
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
	[[nodiscard]] dataframe::cell_reference get_value(
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

	[[nodiscard]] const std::string_view &getColIndex() const
	{
		return sid;
	}

	[[nodiscard]] bool operator==(const SeriesIndex &rhs) const
	{
		return sid == rhs.sid && aggr == rhs.aggr;
	}

	[[nodiscard]] bool operator<(const SeriesIndex &rhs) const
	{
		return sid < rhs.sid || (sid == rhs.sid && aggr < rhs.aggr);
	}

	[[nodiscard]] bool isDimension() const { return !aggr; }

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
	constexpr static auto True = [](const RowWrapper *)
	{
		return true;
	};

	Filter(Fun *lhs, Fun *rhs) noexcept :
	    func1{std::shared_ptr<void>{}, lhs},
	    func2{std::shared_ptr<void>{}, rhs}
	{}

public:
	Filter() noexcept = default;

	template <template <class, class...> class PointerType,
	    class... Types>
	explicit Filter(PointerType<Fun, Types...> &&wr) :
	    func1{std::move(wr)}
	{}

	[[nodiscard]] bool operator==(
	    const Filter &other) const = default;

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		auto &&[min, max] =
		    std::minmax({func1.get(), other.func1.get()});
		auto &&first = min == True ? max : min;
		return {first, max == first ? True : max};
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

	[[nodiscard]] bool operator<(const SliceIndex &rhs) const
	{
		return column < rhs.column
		    || (column == rhs.column && value < rhs.value);
	}

	[[nodiscard]] bool operator==(const SliceIndex &) const = default;
};

struct CellInfo
{
	std::vector<std::pair<std::string, std::string>> markerInfo;

	std::string json;
};

struct MultiIndex
{
	std::size_t rid;
	std::vector<std::size_t> old;
	std::size_t oldAggr;
};

struct MarkerId
{
	std::optional<SliceIndex> label;
	std::size_t seriesId{};
	std::size_t itemId{};

	[[nodiscard]] bool operator==(const MarkerId &id) const
	{
		return itemId == id.itemId && seriesId == id.seriesId;
	}

	[[nodiscard]] auto operator<=>(const MarkerId &id) const
	{
		if (auto &&cmp = seriesId <=> id.seriesId; std::is_neq(cmp))
			return cmp;
		return itemId <=> id.itemId;
	}
};
}

#endif