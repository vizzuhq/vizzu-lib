#include "channel.h"

#include <cmath>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "base/conv/auto_json.h"
#include "base/conv/tostring.h"
#include "base/refl/auto_enum.h"
#include "base/text/smartstring.h"
#include "chart/options/autoparam.h"
#include "dataframe/old/datatable.h"
#include "dataframe/old/types.h"

namespace Vizzu::Gen
{
std::string ChannelSeriesList::toString() const
{
	std::string res;
	Conv::JSONArr arr{res};
	if (auto &&measure = measureId) arr << *measure;
	for (auto &&dim : dimensionIds) arr << dim;
	return res;
}

ChannelSeriesList::Parser &ChannelSeriesList::Parser::operator()(
    const std::string &str)
{
	if (current) {
		if (auto &curr = channels[*current]; path[4] == "name")
			curr.name = str;
		else if (path[4] == "aggregator")
			curr.aggregator = str;
		else
			throw std::runtime_error(
			    Text::SmartString::join<'.'>(path)
			    + ": invalid channel parameter");
	}
	return *this;
}

ChannelSeriesList::Parser &
ChannelSeriesList::Parser::instance() noexcept
{
	thread_local Parser instance;
	return instance;
}

ChannelSeriesList &ChannelSeriesList::operator=(Parser &parser)
{
	if (!parser.current) {
		for (auto &[name, aggr] :
		    std::exchange(parser.channels, {})) {
			std::optional<Data::SeriesIndex> index;

			if (!name) {
				if (aggr.empty())
					throw std::runtime_error(
					    "Empty series at channel: " + parser.path[1]);
				throw std::runtime_error(
				    "Aggregator has no set name at channel "
				    + parser.path[1] + ": " + aggr);
			}

			if (name->empty())
				index.emplace().setAggr(aggr);
			else if (auto &ix = index.emplace(*name, *parser.table);
			         !aggr.empty())
				ix.setAggr(aggr);

			if (parser.path[2] == "detach") {
				if (!removeSeries(*index)) {
					if (index->isDimension())
						throw std::runtime_error(
						    "Missing detachable dimension at channel "
						    + parser.path[1] + ": "
						    + index->getColIndex());

					throw std::runtime_error(
					    "Missing detachable measure at channel "
					    + parser.path[1] + ": "
					    + std::string{Conv::toString(
					        index->getAggr())}
					    + "(" + index->getColIndex() + ")");
				}
			}
			else if (!addSeries(*index)) {
				if (index->isDimension()) {
					throw std::runtime_error(
					    "Multiple dimension at channel "
					    + parser.path[1] + ": "
					    + index->getColIndex());
				}

				throw std::runtime_error(
				    "Multiple measure at channel " + parser.path[1]
				    + ": "
				    + std::string{Conv::toString(
				        measureId->getAggr())}
				    + "(" + measureId->getColIndex() + ") and "
				    + std::string{Conv::toString(index->getAggr())}
				    + "(" + index->getColIndex() + ")");
			}
		}
	}
	return *this;
}

Channel Channel::makeChannel(ChannelId id)
{
	static constexpr auto defStackable =
	    Refl::EnumArray<ChannelId, bool>::make(
	        {{ChannelId::color, false},
	            {ChannelId::lightness, false},
	            {ChannelId::size, true},
	            {ChannelId::label, false},
	            {ChannelId::x, true},
	            {ChannelId::y, true},
	            {ChannelId::noop, false}});
	return {defStackable[id]};
}

bool ChannelSeriesList::addSeries(const Data::SeriesIndex &index)
{
	if (index.isDimension()) return dimensionIds.push_back(index);
	if (measureId) return false;
	measureId = index;
	return true;
}

bool ChannelSeriesList::removeSeries(const Data::SeriesIndex &index)
{
	if (index.isDimension()) return dimensionIds.remove(index);
	auto res = measureId == index;
	if (res) measureId = std::nullopt;
	return res;
}

bool Channel::isSeriesUsed(const Data::SeriesIndex &index) const
{
	return set.measureId == index || set.dimensionIds.contains(index);
}

void Channel::reset()
{
	set = {};
	labelLevel = {};
	title = {};
	axis = Base::AutoBool();
	labels = Base::AutoBool();
	ticks = Base::AutoBool();
	guides = Base::AutoBool();
	markerGuides = Base::AutoBool();
	interlacing = Base::AutoBool();
}

bool Channel::isEmpty() const
{
	return !set.measureId && set.dimensionIds.empty();
}

bool Channel::hasDimension() const
{
	return !set.dimensionIds.empty();
}

bool Channel::hasMeasure() const { return set.measureId.has_value(); }

void Channel::collectDimensions(IndexSet &dimensions) const
{
	for (const auto &dimension : set.dimensionIds)
		dimensions.insert(dimension);
}

}