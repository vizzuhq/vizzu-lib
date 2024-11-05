#include "channel.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "base/conv/auto_json.h"
#include "base/conv/tostring.h"
#include "base/refl/auto_enum.h"
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
	switch (type) {
	default:
	case Token::null: break;
	case Token::name:
		if (!str.empty()) {
			if (res && !res->isDimension())
				res = Data::SeriesIndex{str, *table}.setAggr(
				    res->getAggr());
			else
				res.emplace(str, *table);
		}
		else if (!res)
			res.emplace();
		break;
	case Token::aggregator:
		if (res)
			res->setAggr(str);
		else
			res.emplace().setAggr(str);
		break;
	}
	return *this;
}

ChannelSeriesList::Parser &
ChannelSeriesList::Parser::instance() noexcept
{
	thread_local Parser instance;
	return instance;
}

ChannelSeriesList &ChannelSeriesList::operator=(Parser &index)
{
	if (!index.res) return *this;
	if (auto already_set =
	        dimensionIds.size() + measureId.has_value();
	    already_set == index.position) {
		if (index.res->isDimension()) {
			if (dimensionIds.push_back(*index.res)) index.res = {};
		}
		else if (!measureId)
			measureId = *index.res;
		else
			throw std::runtime_error(
			    "Multiple measure at channel "
			    + std::string{Conv::toString(index.latestChannel)}
			    + ": "
			    + std::string{Conv::toString(measureId->getAggr())}
			    + "(" + measureId->getColIndex() + ") and "
			    + std::string{Conv::toString(index.res->getAggr())}
			    + "("
			    + (index.res->getColIndex().empty()
			            ? "..."
			            : index.res->getColIndex())
			    + ")");
	}
	else if (already_set == index.position + 1) {
		if (!measureId) {
			measureId = dimensionIds.pop_back();
			measureId->setAggr(index.res->getAggr());
			index.res = {};
		}
		else if (measureId->getColIndex() == index.res->getColIndex()
		         || (measureId->getAggr() == index.res->getAggr()
		             && measureId->getColIndex().empty())) {

			measureId = *index.res;
			index.res = {};
		}
		else
			throw std::runtime_error(
			    "Multiple measure at channel "
			    + std::string{Conv::toString(index.latestChannel)}
			    + ": "
			    + std::string{Conv::toString(measureId->getAggr())}
			    + "(" + measureId->getColIndex() + ") and "
			    + std::string{Conv::toString(index.res->getAggr())}
			    + "("
			    + (index.res->getColIndex().empty()
			                && !dimensionIds.empty()
			            ? dimensionIds.pop_back().getColIndex()
			            : index.res->getColIndex())
			    + ")");
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

void Channel::addSeries(const Data::SeriesIndex &index)
{
	if (index.isDimension())
		set.dimensionIds.push_back(index);
	else
		set.measureId = index;
}

void Channel::removeSeries(const Data::SeriesIndex &index)
{
	if (index.isDimension())
		set.dimensionIds.remove(index);
	else if (set.measureId)
		set.measureId = std::nullopt;
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

bool Channel::isDimension() const { return !set.measureId; }

bool Channel::hasDimension() const
{
	return !set.dimensionIds.empty();
}

bool Channel::isMeasure() const { return set.measureId.has_value(); }

void Channel::collectDimensions(IndexSet &dimensions) const
{
	for (const auto &dimension : set.dimensionIds)
		dimensions.insert(dimension);
}

std::pair<const Channel::DimensionIndices &, const std::size_t &>
Channel::dimensionsWithLevel() const
{
	return {set.dimensionIds, labelLevel};
}

Channel::OptionalIndex Channel::labelSeries() const
{
	if (isDimension() && labelLevel < set.dimensionIds.size())
		return *std::next(set.dimensionIds.begin(),
		    static_cast<std::intptr_t>(labelLevel));
	return set.measureId;
}

}