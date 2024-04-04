//
// Created by bela on 2024.02.29..
//

#ifndef DATAFRAME_OLD2_DATATABLE_H
#define DATAFRAME_OLD2_DATATABLE_H

#include "../impl/dataframe.h"

namespace Vizzu::Data
{

struct column_index
{
	dataframe::dataframe_interface::series_identifier sid;
	std::optional<std::size_t> orig_index;
};

struct column_index_wrapper
{
	column_index ncix;
};

bool operator<(const column_index_wrapper &lhs,
    const column_index_wrapper &rhs);

bool operator==(const column_index_wrapper &lhs,
    const column_index_wrapper &rhs);

class data_table
{
public:
	struct cell_t
	{
		dataframe::cell_value cell;

		explicit operator std::string() const;

		explicit operator double() const;
	};

	struct Row
	{
		dataframe::dataframe_interface::record_type rid;
		const data_table *parent;

		auto operator[](column_index_wrapper colIx) const -> cell_t;

		std::size_t size() const;
	};

	struct DataIndex
	{
		using OptColIndex = std::optional<column_index_wrapper>;
		using Type = dataframe::series_type;

		OptColIndex value;
		Type type;

		bool isInvalid() const;
	};

	auto getColumn(const std::string &name) const
	    -> column_index_wrapper;

	struct column_info
	{
		const dataframe::dataframe *dfif;
		dataframe::dataframe_interface::series_identifier sid;

		std::string getUnit() const;
	};

	struct cell_wrapper
	{
		dataframe::cell_value cell;

		cell_wrapper(cell_t const &o,
		    [[maybe_unused]] column_info const &info) :
		    cell(o.cell)
		{}

		bool isDimension() const;

		const char *dimensionValue() const;

		double operator*() const;
	};

	auto getInfo(column_index_wrapper const &colIx) const
	{
		return column_info{&df, colIx.ncix.sid};
	}

	auto getRowCount() const -> std::size_t;

	Row operator[](std::size_t row) const
	{
		return {{&df, row}, this};
	}

	DataIndex getIndex(column_index_wrapper const &col) const;

	void addColumn(const std::string &name,
	    const std::string &unit,
	    const std::span<const double> &values);

	void addColumn(const std::string &name,
	    const std::span<const char *const> &categories,
	    const std::span<const std::uint32_t> &values);

	void pushRow(const std::span<const char *> &cells);

	[[nodiscard]] std::string getInfos() const;

	const dataframe::dataframe &getDf() const { return df; }

private:
	dataframe::dataframe df;
};

class aggregator_t
{
public:
	using Type = dataframe::aggregator_type;

	aggregator_t(std::optional<double> my_res) : my_res(my_res) {}

	explicit operator double() const;

private:
	std::optional<double> my_res;
};

struct series_type_t
{
	dataframe::dataframe_interface::series_identifier sid;
	std::optional<dataframe::aggregator_type> aggr;

	auto aggregatorType() const -> dataframe::aggregator_type;

	bool isDimension() const;

	bool isReal() const;

	bool operator==(const dataframe::aggregator_type &) const;
};

class series_index_t
{
	column_index cix;
	std::optional<dataframe::aggregator_type> aggr;

public:
	using OptColIndex = std::optional<column_index_wrapper>;

	template <class Table>
	series_index_t(std::string const &str, const Table &table);

	template <class DI>
	    requires(requires(DI const &di) { di.type; })
	explicit series_index_t(DI const &dataIndex);

	const std::optional<dataframe::aggregator_type> &getAggr() const
	{
		return aggr;
	}

	[[nodiscard]] auto getType() const -> series_type_t;

	[[nodiscard]] auto getColIndex() const -> OptColIndex;

	template <class Table>
	    requires(requires(Table const &table) { table.getDf(); })
	[[nodiscard]] std::string toString(const Table &table) const;

	friend bool operator==(const series_index_t &lhs,
	    const series_index_t &rhs);
	friend bool operator<(const series_index_t &lhs,
	    const series_index_t &rhs);
};

bool operator==(const series_index_t &lhs, const series_index_t &rhs);

bool operator<(const series_index_t &lhs, const series_index_t &rhs);

struct slice_index
{
	std::string_view column;
	std::string_view value;
	std::size_t orig_index;
	std::size_t orig_value;
};

struct slice_index_t
{
	slice_index new_;

	[[nodiscard]] bool operator<(slice_index_t const &) const;

	[[nodiscard]] bool operator==(const slice_index_t &) const;
};

using subslice_index_t = std::vector<slice_index_t>;

struct multi_index_t
{
	const dataframe::dataframe_interface *parent;
	std::optional<dataframe::dataframe_interface::record_identifier>
	    rid;
	const std::vector<std::size_t> *dim_reindex;
	std::vector<std::size_t> old;

	bool empty() const;
};

struct data_cube_cell_t
{
	const dataframe::dataframe_interface *parent;
	std::optional<dataframe::dataframe_interface::record_identifier>
	    rid;

	bool isEmpty() const;
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
		std::size_t seriesId;
		std::size_t itemId;

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
			std::size_t found_count{};

			iterator_t(
			    dataframe::dataframe_interface::record_type rid,
			    const data_t *parent,
			    std::size_t old);

			void incr();

			[[nodiscard]] bool operator!=(
			    const iterator_t &oth) const;

			iterator_t &operator++();

			MultiIndex getIndex() const;
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

		template <class MI>
		auto at(const MI &index) const -> data_cube_cell_t;

		std::vector<std::size_t> get_indices(std::size_t ix) const;

		auto begin() const -> iterator_t;
		auto end() const -> iterator_t;
	};

	const data_table *table;
	std::shared_ptr<dataframe::dataframe_interface> df;
	std::shared_ptr<dataframe::dataframe_interface> removed;
	std::map<
	    std::pair<dataframe::dataframe_interface::series_identifier,
	        std::string>,
	    std::string>
	    measure_names;
	data_t data;

	std::shared_ptr<std::map<
	    std::set<dataframe::dataframe_interface::series_identifier>,
	    std::shared_ptr<dataframe::dataframe_interface>>>
	    cacheImpl = std::make_shared<
	        typename decltype(cacheImpl)::element_type>();

	template <class Table, class Options, class Filter>
	data_cube_t(const Table &table,
	    const Options &options,
	    const Filter &filter);

	template <class SL>
	[[nodiscard]] size_t combinedSizeOf(const SL &colIndices) const;

	[[nodiscard]] size_t subCellSize() const;

	[[nodiscard]] const data_t &getData() const { return data; }

	template <class MI>
	[[nodiscard]] auto cellInfo(const MI &index) const -> CellInfo;

	template <class MI, class SL, class SI>
	[[nodiscard]] auto aggregateAt(const MI &multiIndex,
	    const SL &sumCols,
	    SI seriesId) const -> aggregator_t;

	template <class MI, class SI>
	[[nodiscard]] auto valueAt(const MI &multiIndex,
	    const SI &seriesId) const -> aggregator_t;

	auto getTable() const -> const data_table * { return table; }

	template <class SL, class MI>
	[[nodiscard]] Id getId(const SL &, const MI &) const;

	template <class SI>
	    requires(requires(SI const &si) { si.new_; })
	[[nodiscard]] std::string getValue(const SI &index,
	    std::string def = "") const;
};

}

template <> struct std::hash<Vizzu::Data::column_index_wrapper>
{
	auto operator()(
	    const Vizzu::Data::column_index_wrapper &o) const noexcept
	{
		return std::hash<Vizzu::dataframe::dataframe_interface::
		        series_identifier>{}(o.ncix.sid);
	}
};

#endif // DATAFRAME_OLD2_DATATABLE_H
