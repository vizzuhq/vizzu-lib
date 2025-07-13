
#ifndef DATAFRAME_OLD_TYPES_H
#define DATAFRAME_OLD_TYPES_H

#include "../interface.h"
#include "base/type/uniquelist.h"

namespace Vizzu::dataframe
{
struct series_meta_t;
class dataframe;
}

namespace Vizzu::Data
{
struct DataTable;
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
	std::string name{};
	std::optional<dataframe::aggregator_type> aggregator;

	explicit SeriesIndex(dataframe::series_meta_t const &meta);

public:
	SeriesIndex() noexcept :
	    aggregator(dataframe::aggregator_type::count)
	{}
	SeriesIndex(std::string const &str, const DataTable &table);

	void setAggr(const std::string &aggr);

	SeriesIndex &setAggr(
	    const std::optional<dataframe::aggregator_type> &aggr)
	{
		aggregator = aggr;
		return *this;
	}

	[[nodiscard]] const dataframe::aggregator_type &getAggr() const
	{
		return *aggregator;
	}

	[[nodiscard]] const std::string &getColIndex() const
	{
		return name;
	}

	[[nodiscard]] bool operator==(
	    const SeriesIndex &rhs) const = default;
	[[nodiscard]] auto operator<=>(
	    const SeriesIndex &rhs) const = default;

	[[nodiscard]] bool isDimension() const { return !aggregator; }

	[[nodiscard]] std::string toJSON() const;
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

	[[nodiscard]] Fun *getFun1() const
	{
		return func1.get() == True ? nullptr : func1.get();
	}
	[[nodiscard]] Fun *getFun2() const
	{
		return func2.get() == True ? nullptr : func2.get();
	}

private:
	SharedFun func1{std::shared_ptr<void>{}, True};
	SharedFun func2{std::shared_ptr<void>{}, True};
};

struct SliceIndex
{
	std::string column;
	std::string value;

	[[nodiscard]] auto operator<=>(
	    const SliceIndex &rhs) const = default;
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
	std::string marker_id;
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

struct DataTable
{
	[[nodiscard]] virtual std::string_view get_series_info(
	    const std::string &id,
	    const char *key) const & = 0;

	[[nodiscard]] virtual dataframe::series_meta_t get_series_meta(
	    const std::string &id) const & = 0;

	virtual void add_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    std::string_view name,
	    std::span<const std::pair<const char *, const char *>> info,
	    dataframe::adding_type adding_strategy) & = 0;

	virtual void add_measure(std::span<const double> measure_values,
	    std::string_view name,
	    std::span<const std::pair<const char *, const char *>> info,
	    dataframe::adding_type adding_strategy) & = 0;

	virtual void add_record(
	    std::span<const char *const> values) & = 0;

	[[nodiscard]] virtual std::string as_string() const & = 0;

	[[nodiscard]] virtual std::pair<
	    std::shared_ptr<dataframe::dataframe_interface>,
	    std::map<SeriesIndex, std::string>>
	aggregate(const Filter &filter,
	    const std::set<SeriesIndex> &aggregate_by,
	    const std::set<SeriesIndex> &aggregating) const & = 0;

	virtual ~DataTable() = default;
};

struct DataTableImpl final : DataTable
{
	std::shared_ptr<dataframe::dataframe_interface> df;

	explicit DataTableImpl(
	    std::shared_ptr<dataframe::dataframe_interface> &&df) noexcept
	    :
	    df(std::move(df))
	{}

	[[nodiscard]] std::string_view get_series_info(
	    const std::string &id,
	    const char *key) const & override
	{
		return df->get_series_info(id, key);
	}

	[[nodiscard]] dataframe::series_meta_t get_series_meta(
	    const std::string &id) const & override
	{
		return df->get_series_meta(id);
	}

	void add_dimension(
	    std::span<const char *const> dimension_categories,
	    std::span<const std::uint32_t> dimension_values,
	    std::string_view name,
	    std::span<const std::pair<const char *, const char *>> info,
	    dataframe::adding_type adding_strategy)
	    & override
	{
		df->add_dimension(dimension_categories,
		    dimension_values,
		    name,
		    info,
		    adding_strategy);
	}

	void add_measure(std::span<const double> measure_values,
	    std::string_view name,
	    std::span<const std::pair<const char *, const char *>> info,
	    dataframe::adding_type adding_strategy)
	    & override
	{
		df->add_measure(measure_values, name, info, adding_strategy);
	}

	void add_record(std::span<const char *const> values) & override
	{
		df->add_record(values);
	}

	[[nodiscard]] std::string as_string() const & override
	{
		return df->as_string();
	}

	[[nodiscard]] std::pair<
	    std::shared_ptr<dataframe::dataframe_interface>,
	    std::map<SeriesIndex, std::string>>
	aggregate(const Filter &filter,
	    const std::set<SeriesIndex> &aggregate_by,
	    const std::set<SeriesIndex> &aggregating) const & override
	{
		auto &&res = std::pair{df->copy(false),
		    std::map<SeriesIndex, std::string>{}};

		res.first->remove_records(filter.getFunction());

		for (const auto &dim : aggregate_by)
			res.first->aggregate_by(dim.getColIndex());

		for (const auto &meas : aggregating)
			res.second.try_emplace(meas,
			    res.first->set_aggregate(meas.getColIndex(),
			        meas.getAggr()));

		for (const auto &dim : aggregate_by)
			res.first->set_sort(dim.getColIndex(),
			    dataframe::sort_type::less,
			    dataframe::na_position::first);

		res.first->finalize();
		return res;
	}
};

}

#endif