//
// Created by bela on 2024.02.29..
//

#ifndef DATAFRAME_OLD2_DATATABLE_H
#define DATAFRAME_OLD2_DATATABLE_H

#include <base/type/uniquelist.h>

#include "../impl/dataframe.h"

namespace Vizzu::Data
{

class RowWrapper
{
public:
	dataframe::dataframe_interface::record_type rid;
	[[nodiscard]] dataframe::cell_value operator[](
	    std::string_view const &col) const;
};

class data_table
{
public:
	using Type = dataframe::series_type;
	[[nodiscard]] std::string_view getColumn(
	    const std::string &name) const;

	[[nodiscard]] std::string_view getUnit(
	    std::string_view const &colIx) const
	{
		return df.get_series_info(colIx, "unit");
	}

	[[nodiscard]] Type getType(const std::string_view &cix) const
	{
		return df.get_series_type(cix);
	}

	[[nodiscard]] std::size_t getRowCount() const;

	void addColumn(const std::string &name,
	    const std::string &unit,
	    const std::span<const double> &values);

	void addColumn(const std::string &name,
	    const std::span<const char *const> &categories,
	    const std::span<const std::uint32_t> &values);

	void pushRow(const std::span<const char *> &cells);

	[[nodiscard]] std::string getInfos() const;

	[[nodiscard]] const dataframe::dataframe &getDf() const
	{
		return df;
	}

private:
	dataframe::dataframe df;
};

class series_index_t
{
	std::string orig_name;
	std::string_view sid;
	std::optional<std::size_t> orig_index;
	std::optional<dataframe::aggregator_type> aggr;

public:
	using OptColIndex = std::string_view;

	series_index_t(std::string const &str, const data_table &table);

	[[nodiscard]] const dataframe::aggregator_type &getAggr() const
	{
		return *aggr;
	}

	[[nodiscard]] const OptColIndex &getColIndex() const;

	friend bool operator==(const series_index_t &lhs,
	    const series_index_t &rhs);
	friend bool operator<(const series_index_t &lhs,
	    const series_index_t &rhs);

	[[nodiscard]] bool isDimension() const;

	[[nodiscard]] const std::string &getOrigName() const
	{
		return orig_name;
	}
};

bool operator==(const series_index_t &lhs, const series_index_t &rhs);

bool operator<(const series_index_t &lhs, const series_index_t &rhs);

using series_index_list_t = Type::UniqueList<series_index_t>;

struct slice_index_t
{
	std::string_view column;
	std::string_view value;
	std::size_t orig_index;
	std::size_t orig_value;

	[[nodiscard]] bool operator<(slice_index_t const &) const;

	[[nodiscard]] bool operator==(const slice_index_t &) const;
};

using subslice_index_t = std::vector<slice_index_t>;

struct multi_index_t
{
	const dataframe::dataframe_interface *parent{};
	std::optional<dataframe::dataframe_interface::record_identifier>
	    rid;
	const std::vector<std::size_t> *dim_reindex{};
	std::vector<std::size_t> old;

	[[nodiscard]] bool empty() const;

	[[nodiscard]] bool isEmpty() const;
};

class data_cube_t
{
public:
	using MultiIndex = multi_index_t;
	struct Id
	{
		using SubSliceIndex = subslice_index_t;
		using SliceIndex = std::ranges::range_value_t<SubSliceIndex>;

		MultiIndex mi;
		SubSliceIndex itemSliceIndex;
		std::size_t seriesId{};
		std::size_t itemId{};

		[[nodiscard]] bool operator==(const Id &) const;
	};

	struct CellInfo
	{
		std::map<std::string_view, std::string_view> categories;
		std::map<std::string_view, double> values;
	};

	class data_t
	{
		struct iterator_t
		{
			dataframe::dataframe_interface::record_type rid;
			const data_t *parent;
			std::size_t old;
			bool found{};

			iterator_t(
			    dataframe::dataframe_interface::record_type rid,
			    const data_t *parent,
			    std::size_t old);

			void incr();

			[[nodiscard]] bool operator!=(
			    const iterator_t &oth) const;

			iterator_t &operator++();

			[[nodiscard]] MultiIndex getIndex() const;
		};
		dataframe::dataframe_interface *df;

	public:
		std::vector<std::size_t> dim_reindex;
		std::vector<std::size_t> sizes;
		std::size_t full_size{};

		template <class Options>
		explicit data_t(dataframe::dataframe_interface *df,
		    const Options &options) :
		    df(df),
		    dim_reindex(options.getDimensions().size()),
		    sizes(options.getDimensions().size())
		{}

		[[nodiscard]] std::vector<std::size_t> get_indices(
		    std::size_t ix) const;

		[[nodiscard]] iterator_t begin() const;
		[[nodiscard]] iterator_t end() const;
	};

	const data_table *table;
	std::shared_ptr<dataframe::dataframe_interface> df;
	std::shared_ptr<dataframe::dataframe_interface> removed;
	std::map<std::pair<std::string_view, std::string>, std::string>
	    measure_names;
	data_t data;

	std::shared_ptr<std::map<std::set<std::string_view>,
	    std::shared_ptr<dataframe::dataframe_interface>>>
	    cacheImpl = std::make_shared<
	        typename decltype(cacheImpl)::element_type>();

	template <class Options, class Filter>
	data_cube_t(const data_table &table,
	    const Options &options,
	    const Filter &filter);

	[[nodiscard]] size_t combinedSizeOf(
	    const series_index_list_t &colIndices) const;

	[[nodiscard]] size_t subCellSize() const;

	[[nodiscard]] const data_t &getData() const { return data; }

	[[nodiscard]] CellInfo cellInfo(const multi_index_t &index) const;

	[[nodiscard]] double aggregateAt(const multi_index_t &multiIndex,
	    const series_index_list_t &sumCols,
	    const series_index_t &seriesId) const;

	[[nodiscard]] double valueAt(const multi_index_t &multiIndex,
	    const series_index_t &seriesId) const;

	[[nodiscard]] const data_table *getTable() const { return table; }

	[[nodiscard]] Id getId(const series_index_list_t &,
	    const multi_index_t &) const;

	[[nodiscard]] std::string getValue(const slice_index_t &index,
	    std::string &&def = "") const;

	[[nodiscard]] const std::string &getName(
	    const series_index_t &seriesId) const;
};

}

#endif // DATAFRAME_OLD2_DATATABLE_H