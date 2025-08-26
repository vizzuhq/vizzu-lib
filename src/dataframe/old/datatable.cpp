#include "datatable.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "base/conv/auto_json.h"
#include "base/conv/numtostr.h"
#include "base/conv/tostring.h"
#include "base/refl/auto_enum.h"
#include "base/text/smartstring.h"
#include "chart/options/options.h"
#include "chart/options/shapetype.h"
#include "dataframe/impl/data_source.h"
#include "dataframe/interface.h"

#include "types.h"

namespace Vizzu::Data
{
bool DataCube::iterator_t::operator!=(const iterator_t &other) const
{
	return parent != other.parent;
}

void DataCube::iterator_t::operator++() { parent->incr(*this); }

const MultiIndex &DataCube::iterator_t::operator*() const
{
	return index;
}

SeriesIndex::SeriesIndex(dataframe::series_meta_t const &meta) :
    name{meta.name}
{
	if (meta.type == dataframe::series_type::measure)
		aggregator.emplace(dataframe::aggregator_type::sum);
}

SeriesIndex::SeriesIndex(std::string const &str,
    const DataTable &table) :
    SeriesIndex(table.get_series_meta(str))
{}

void SeriesIndex::setAggr(const std::string &aggr)
{
	aggregator = Refl::get_enum<dataframe::aggregator_type>(aggr);
}

std::string SeriesIndex::toJSON() const
{
	std::string res;
	Conv::JSONObj obj{res};
	obj("name", name);
	if (aggregator) obj("aggregator", Conv::toString(*aggregator));
	return res;
}

DataCube::iterator_t DataCube::begin() const
{
	iterator_t res{this,
	    {{}, std::vector<std::size_t>(dim_reindex.size()), {}}};
	check(res);
	return res;
}

DataCube::iterator_t DataCube::end() { return {}; }

void DataCube::check(iterator_t &it) const
{
	if (it.index.rid == df->get_record_count()) {
		it.parent = nullptr;
		return;
	}

	it.index.marker_id = df->get_record_id(it.index.rid);
	for (auto &&[dim, cats, size, ix] : dim_reindex) {
		const auto *str_ptr = std::get<const std::string *>(
		    df->get_data(it.index.rid, dim));

		it.index.old[ix] =
		    str_ptr == nullptr ? cats.size() : str_ptr - cats.data();
	}
}

void DataCube::incr(iterator_t &it) const
{
	++it.index.rid;
	check(it);
}

DataCube::DataCube(const DataTable &table,
    const Gen::Options &options)
{
	auto &&channels = options.getChannels();
	auto &&dimensions = channels.getDimensions();
	auto &&measures = channels.getMeasures();

	if (dimensions.empty() && measures.empty()) {
		df = dataframe::dataframe::create_new();
		df->finalize();
		return;
	}

	std::tie(df, measure_names) =
	    table.aggregate(options.dataFilter, dimensions, measures);

	for (std::size_t ix{}; const auto &dim : dimensions) {
		auto &&dimName = dim.getColIndex();
		auto &&cats = df->get_categories(dimName);
		dim_reindex.push_back(DimensionInfo{dimName,
		    cats,
		    cats.size() + df->has_na(dimName),
		    ix++});
	}

	auto stackInhibitingShape =
	    options.geometry == Gen::ShapeType::area;
	auto horizontal = options.isHorizontal();

	using ChannelId = Gen::ChannelId;
	for (auto &&[channelId, inhibitStack] :
	    std::initializer_list<std::pair<ChannelId, bool>>{
	        {ChannelId::size, false},
	        {ChannelId::x, !horizontal && stackInhibitingShape},
	        {ChannelId::y, horizontal && stackInhibitingShape}}) {
		const auto &channel = channels.at(channelId);
		auto &&meas = channel.measure();
		if (!meas) continue;
		const auto *subChannel = options.subAxisOf(channelId);
		if (!subChannel) continue;
		auto sumBy = subChannel->dimensions();
		if (auto &&common = sumBy.split_by(channel.dimensions());
		    inhibitStack)
			std::swap(sumBy, common);
		if (sumBy.empty()) continue;

		auto dimCp{dimensions};
		std::erase_if(dimCp,
		    [&sumBy](const auto &dim)
		    {
			    return sumBy.contains(dim);
		    });

		cacheImpl.try_emplace(channelId,
		    table.aggregate(options.dataFilter, dimCp, {*meas})
		        .first);
	}
}

bool DataCube::empty() const
{
	return df->get_measures().empty() && df->get_dimensions().empty();
}

const std::string &DataCube::getName(
    const SeriesIndex &seriesId) const
{
	return measure_names.at(seriesId);
}

MarkerId DataCube::getId(const SeriesList &sl,
    const std::optional<std::size_t> &ll,
    const MultiIndex &mi) const
{
	MarkerId res{};
	std::vector<std::pair<std::size_t, std::size_t>> v(sl.size());

	for (auto &&[val, comm] : dim_reindex.iterate_common(sl)) {
		auto &&[name, cats, size, ix] = val;
		auto &&oldIx = mi.old[ix];
		if (comm) {
			if (v[*comm] = {oldIx, size}; *comm == ll)
				res.label.emplace(name,
				    oldIx < cats.size() ? cats[oldIx]
				                        : std::string{});
		}
		else
			res.seriesId = res.seriesId * size + oldIx;
	}

	for (const auto &[cix, size] : v)
		res.itemId = res.itemId * size + cix;

	return res;
}

std::string DataCube::joinDimensionValues(const SeriesList &sl,
    const MultiIndex &index) const
{
	std::vector<std::string_view> resColl(sl.size());
	for (auto &&[val, comm] : dim_reindex.iterate_common(sl))
		if (comm) {
			auto &&[name, cats, size, ix] = val;
			if (auto &&oldIx = index.old[ix]; oldIx < cats.size())
				resColl[*comm] = cats[oldIx];
		}

	return Text::SmartString::join<',', ' '>(resColl);
}

std::shared_ptr<const CellInfo>
DataCube::cellInfo(const MultiIndex &index, bool needMarkerInfo) const
{
	auto my_res = std::make_shared<CellInfo>();
	if (needMarkerInfo)
		my_res->markerInfo.reserve(
		    dim_reindex.size() + df->get_measures().size());

	Conv::JSONObj obj{my_res->json};
	obj("index", index.marker_id);
	for (Conv::JSONObj &&dims{obj.nested("categories")};
	     auto &&[name, cats, size, ix] : dim_reindex) {
		auto &&cix = index.old[ix];
		auto &&cat = cix < cats.size() ? cats[cix] : std::string{};
		dims.key<false>(name).primitive(cat);
		if (needMarkerInfo)
			my_res->markerInfo.emplace_back(name, cat);
	}

	for (Conv::JSONObj &&vals{obj.nested("values")};
	     auto &&meas : df->get_measures()) {
		auto val = std::get<double>(df->get_data(index.rid, meas));
		vals.key<false>(meas).primitive(val);
		if (needMarkerInfo) {
			thread_local auto conv =
			    Conv::NumberToString{.fractionDigitCount = 3};
			my_res->markerInfo.emplace_back(meas, conv(val));
		}
	}
	return my_res;
}

double DataCube::valueAt(const MultiIndex &multiIndex,
    const SeriesIndex &seriesId) const
{
	return std::get<double>(
	    df->get_data(multiIndex.rid, getName(seriesId)));
}

double DataCube::aggregateAt(const MultiIndex &multiIndex,
    const Gen::ChannelId &channelId,
    const SeriesIndex &seriesId) const
{
	auto it = cacheImpl.find(channelId);
	if (it == cacheImpl.end()) return valueAt(multiIndex, seriesId);

	auto &sub_df = *it->second;

	return std::get<double>(
	    sub_df.get_data(df->get_record_id_by_dims(multiIndex.rid,
	                        sub_df.get_dimensions()),
	        getName(seriesId)));
}

} // namespace Vizzu::Data
