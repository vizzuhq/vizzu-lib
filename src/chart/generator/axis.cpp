#include "axis.h"

#include "base/math/interpolation.h"
#include "base/math/renard.h"

namespace Vizzu::Gen
{

Geom::Point MeasureAxises::origo() const
{
	return {at(ChannelId::x).origo(), at(ChannelId::y).origo()};
}

MeasureAxis::MeasureAxis(Math::Range<double> interval,
    std::string title,
    std::string unit,
    std::optional<double> step) :
    enabled(true),
    range(interval),
    title(std::move(title)),
    unit(std::move(unit)),
    step(step ? *step : Math::Renard::R5().ceil(range.size() / 5.0))
{}

bool MeasureAxis::operator==(const MeasureAxis &other) const
{
	return enabled == other.enabled && range == other.range
	    && step == other.step && unit == other.unit
	    && title == other.title;
}

double MeasureAxis::origo() const
{
	if (range.size() == 0) return 0;
	return -range.getMin() / range.size();
}

MeasureAxis interpolate(const MeasureAxis &op0,
    const MeasureAxis &op1,
    double factor)
{
	MeasureAxis res;
	res.enabled = interpolate(op0.enabled, op1.enabled, factor);

	if (op0.enabled.get() && op1.enabled.get()) {
		res.range = Math::interpolate(op0.range, op1.range, factor);
		res.step = interpolate(op0.step, op1.step, factor);
	}
	else if (op0.enabled.get()) {
		res.range = op0.range;
		res.step = op0.step;
	}
	else if (op1.enabled.get()) {
		res.range = op1.range;
		res.step = op1.step;
	}

	res.unit = op1.unit;
	res.title = interpolate(op0.title, op1.title, factor);

	return res;
}
bool DimensionAxis::add(const Data::MultiDim::SliceIndex &index,
    double value,
    Math::Range<double> &range,
    double enabled,
    bool merge)
{
	if (enabled <= 0) return false;

	this->enabled = true;

	if (merge) {
		if (auto it = values.find(index); it != values.end()) {
			it->second.range.include(range);
			it->second.weight = std::max(it->second.weight, enabled);
			return false;
		}
	}
	values.emplace(std::piecewise_construct,
	    std::tuple{index},
	    std::tuple{range, value, enabled});

	return true;
}

bool DimensionAxis::operator==(const DimensionAxis &other) const
{
	return enabled == other.enabled && values == other.values;
}

void DimensionAxis::setLabels(const Data::DataCube &data,
    const Data::DataTable &table)
{
	Values::iterator it;
	for (it = values.begin(); it != values.end(); ++it) {
		auto colIndex =
		    data.getSeriesByDim(it->first.dimIndex).getColIndex();
		const auto &categories =
		    table.getInfo(colIndex.value()).categories();
		if (it->first.index < categories.size())
			it->second.label = categories[it->first.index];
		else
			it->second.label = "NA";
	}
}

DimensionAxis interpolate(const DimensionAxis &op0,
    const DimensionAxis &op1,
    double factor)
{
	DimensionAxis res;
	res.title = interpolate(op0.title, op1.title, factor);

	DimensionAxis::Values::const_iterator it;
	for (it = op0.values.cbegin(); it != op0.values.cend(); ++it) {
		res.enabled = true;
		res.values.emplace(std::piecewise_construct,
		    std::tuple{it->first},
		    std::tuple{it->second, true, 1 - factor});
	}

	for (it = op1.values.cbegin(); it != op1.values.cend(); ++it) {
		res.enabled = true;
		auto [resIt, end] = res.values.equal_range(it->first);

		while (resIt != end && resIt->second.end) { ++resIt; }

		if (resIt == end) {
			res.values.emplace_hint(resIt,
			    std::piecewise_construct,
			    std::tuple{it->first},
			    std::tuple{it->second, false, factor});
		}
		else {
			resIt->second.end = true;

			resIt->second.range =
			    Math::interpolate(resIt->second.range,
			        it->second.range,
			        factor);

			resIt->second.colorBase =
			    interpolate(resIt->second.colorBase,
			        it->second.colorBase,
			        factor);

			resIt->second.value =
			    Math::interpolate(resIt->second.value,
			        it->second.value,
			        factor);

			resIt->second.weight += it->second.weight * factor;
		}
	}

	return res;
}

}
