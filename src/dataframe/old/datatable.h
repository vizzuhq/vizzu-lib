//
// Created by bela on 2024.02.29..
//

#ifndef DATAFRAME_OLD2_DATATABLE_H
#define DATAFRAME_OLD2_DATATABLE_H

#include <base/type/uniquelist.h>

#include "../impl/dataframe.h"

namespace Vizzu::Data
{

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

class data_table
{
public:
	using Type = dataframe::series_type;
	[[nodiscard]] std::string_view getUnit(
	    std::string_view const &colIx) const
	{
		return df.get_series_info(colIx, "unit");
	}

	void addColumn(std::string_view name,
	    std::string_view unit,
	    const std::span<const double> &values);

	void addColumn(std::string_view name,
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
	std::optional<dataframe::aggregator_type> aggr;

public:
	series_index_t(std::string const &str, const data_table &table);

	[[nodiscard]] const dataframe::aggregator_type &getAggr() const
	{
		return *aggr;
	}

	[[nodiscard]] const std::string_view &getColIndex() const;

	bool operator==(const series_index_t &rhs) const;
	bool operator<(const series_index_t &rhs) const;

	[[nodiscard]] bool isDimension() const;

	[[nodiscard]] const std::string &getOrigName() const
	{
		return orig_name;
	}
};

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

class data_cube_t
{
public:
	struct CellInfo
	{
		std::map<std::string_view, std::string_view> categories;
		std::map<std::string_view, double> values;
	};

	class data_t
	{
	public:
		struct multi_index_t
		{
			const data_t *parent{};
			std::optional<std::size_t> rid;
			std::vector<std::size_t> old;

			[[nodiscard]] bool has_dimension() const;

			[[nodiscard]] bool isEmpty() const;
		};

	private:
		struct iterator_t
		{
			std::size_t rid{};
			multi_index_t index;

			[[nodiscard]] bool operator!=(
			    const iterator_t &oth) const;

			void operator++();

			[[nodiscard]] const multi_index_t &operator*() const;
		};

	public:
		dataframe::dataframe_interface *df;
		std::vector<std::string_view> dim_reindex;
		std::vector<std::size_t> sizes;

		template <class Options>
		explicit data_t(dataframe::dataframe_interface *df,
		    const Options &options) :
		    df(df),
		    dim_reindex(options.getDimensions().size()),
		    sizes(options.getDimensions().size())
		{}

		[[nodiscard]] iterator_t begin() const;
		[[nodiscard]] static iterator_t end();

		void check(iterator_t &it) const;
		void incr(iterator_t &it) const;
	};
	using multi_index_t = data_t::multi_index_t;
	using MultiIndex = multi_index_t;

	struct Id
	{
		using SubSliceIndex = subslice_index_t;
		using SliceIndex = std::ranges::range_value_t<SubSliceIndex>;

		multi_index_t mi;
		SubSliceIndex itemSliceIndex;
		std::size_t seriesId{};
		std::size_t itemId{};

		[[nodiscard]] bool operator==(const Id &) const;
	};

	const data_table *table;
	std::shared_ptr<dataframe::dataframe_interface> df;
	std::shared_ptr<dataframe::dataframe_interface> removed;
	std::map<std::pair<std::string_view, dataframe::aggregator_type>,
	    std::string>
	    measure_names;
	data_t data;

	std::shared_ptr<std::map<std::string,
	    std::shared_ptr<dataframe::dataframe_interface>>>
	    cacheImpl = std::make_shared<
	        typename decltype(cacheImpl)::element_type>();

	template <class Options, class Filter>
	data_cube_t(const data_table &table,
	    const Options &options,
	    const Filter &filter);

	[[nodiscard]] size_t combinedSizeOf(
	    const series_index_list_t &colIndices) const;

	[[nodiscard]] bool empty() const;

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

	[[nodiscard]] static std::string getValue(
	    const slice_index_t &index);

	[[nodiscard]] const std::string &getName(
	    const series_index_t &seriesId) const;
};

}

#endif // DATAFRAME_OLD2_DATATABLE_H
