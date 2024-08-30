
#ifndef DATAFRAME_OLD_TYPES_H
#define DATAFRAME_OLD_TYPES_H

#include <set>

#include "../interface.h"
#include "base/text/immutable_string.h"
#include "base/type/uniquelist.h"

namespace Vizzu::dataframe
{
struct series_meta_t;
}

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
	std::string_view name{};
	std::optional<dataframe::aggregator_type> aggregator;

	explicit SeriesIndex(dataframe::series_meta_t const &meta);

public:
	SeriesIndex() : aggregator(dataframe::aggregator_type::count) {}
	SeriesIndex(std::string const &str, const DataTable &table);

	void setAggr(const std::string &aggr);

	[[nodiscard]] const dataframe::aggregator_type &getAggr() const
	{
		return *aggregator;
	}

	[[nodiscard]] const Text::immutable_string &getColIndex() const
	{
		return name;
	}

	[[nodiscard]] bool operator==(
	    const SeriesIndex &rhs) const = default;
	[[nodiscard]] auto operator<=>(
	    const SeriesIndex &rhs) const = default;

	[[nodiscard]] bool isDimension() const { return !aggregator; }

	[[nodiscard]] consteval static auto members()
	{
		return std::tuple{&SeriesIndex::name,
		    &SeriesIndex::aggregator};
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
	explicit Filter(PointerType<Fun, Types...> wr) :
	    func1{wr.release(), wr.get_deleter()}
	{}

	[[nodiscard]] bool operator==(
	    const Filter &other) const = default;

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		auto &&[min, max] =
		    std::minmax({func1.get(), other.func1.get()},
		        std::less<Fun *>{});
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
	Text::immutable_string column;
	Text::immutable_string value;

	[[nodiscard]] bool operator<(const SliceIndex &rhs) const
	{
		return column < rhs.column
		    || (column == rhs.column && value < rhs.value);
	}

	[[nodiscard]] bool operator==(const SliceIndex &) const = default;
};

struct CellInfo
{
	std::vector<
	    std::pair<Text::immutable_string, Text::immutable_string>>
	    markerInfo;

	std::string json;
};

struct MultiIndex
{
	std::size_t rid;
	std::vector<std::size_t> old;
	Text::immutable_string marker_id;
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