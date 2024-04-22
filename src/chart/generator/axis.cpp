#include "axis.h"

#include "base/math/interpolation.h"
#include "base/math/renard.h"

namespace Vizzu::Gen
{

CommonAxis interpolate(const CommonAxis &op0,
    const CommonAxis &op1,
    double factor)
{
	return {interpolate(op0.title, op1.title, factor)};
}

Geom::Point MeasureAxises::origo() const
{
	return {at(ChannelId::x).origo(), at(ChannelId::y).origo()};
}

MeasureAxis::MeasureAxis(Math::Range<double> interval,
    std::string &&unit,
    std::optional<double> step) :
    enabled(true),
    range(interval),
    unit(std::move(unit)),
    step(step ? *step : Math::Renard::R5().ceil(range.size() / 5.0))
{}

bool MeasureAxis::operator==(const MeasureAxis &other) const
{
	return enabled == other.enabled && range == other.range
	    && step == other.step && unit == other.unit;
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
		res.unit = interpolate(op0.unit, op1.unit, factor);
	}
	else if (op0.enabled.get()) {
		res.range = op0.range;
		res.step = op0.step;
		res.unit = op0.unit;
	}
	else if (op1.enabled.get()) {
		res.range = op1.range;
		res.step = op1.step;
		res.unit = op1.unit;
	}

	return res;
}
bool DimensionAxis::add(const Data::DataCube::Id::SliceIndex &index,
    double value,
    const Math::Range<double> &range,
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

bool DimensionAxis::setLabels(double step)
{
	bool hasLabel{};
	step = std::max(step, 1.0);
	double currStep = 0.0;

	std::multimap<double, Values::pointer> reorder;
	for (auto &ref : values)
		reorder.emplace(ref.second.range.getMin(), &ref);

	for (int curr{}; auto &[v, pp] : reorder) {
		auto &[slice, item] = *pp;
		item.categoryValue = Data::DataCube::getValue(slice);

		if (++curr <= currStep) continue;
		currStep += step;
		item.label = item.categoryValue;
		hasLabel = true;
	}
	return hasLabel;
}

DimensionAxis interpolate(const DimensionAxis &op0,
    const DimensionAxis &op1,
    double factor)
{
	DimensionAxis res;

	for (const auto &[slice, item] : op0.values) {
		res.enabled = true;
		res.values.emplace(std::piecewise_construct,
		    std::tuple{slice},
		    std::forward_as_tuple(item, true, 1 - factor));
	}

	for (const auto &[slice, item] : op1.values) {
		res.enabled = true;
		auto [resIt, end] = res.values.equal_range(slice);

		while (resIt != end && resIt->second.end) { ++resIt; }

		if (resIt == end) {
			res.values.emplace_hint(resIt,
			    std::piecewise_construct,
			    std::tuple{slice},
			    std::forward_as_tuple(item, false, factor));
		}
		else {
			resIt->second.end = true;

			resIt->second.range =
			    Math::interpolate(resIt->second.range,
			        item.range,
			        factor);

			resIt->second.colorBase =
			    interpolate(resIt->second.colorBase,
			        item.colorBase,
			        factor);

			resIt->second.label =
			    interpolate(resIt->second.label, item.label, factor);

			resIt->second.value =
			    Math::interpolate(resIt->second.value,
			        item.value,
			        factor);

			resIt->second.weight += item.weight * factor;
		}
	}

	return res;
}

}
