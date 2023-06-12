#include "axis.h"

#include "base/io/log.h"
#include "base/math/interpolation.h"
#include "base/math/renard.h"

namespace Vizzu::Diag
{

Geom::Point Axises::origo() const {
	return Geom::Point(
		at(ScaleId::x).origo(),
		at(ScaleId::y).origo());
}

Axis::Axis()
{
	enabled = false;
	range = Math::Range<double>(0,1);
	step = 1.0;
}

Axis::Axis(Math::Range<double> interval, 
	std::string title, 
	std::string unit, 
	std::optional<double> step) :
    range(interval),
	title(std::move(title)),
    unit(std::move(unit))
{
	enabled = true;
	if (step) this->step = *step;
	else this->step = Math::Renard::R5().ceil(range.size() / 5.0);
}

bool Axis::operator==(const Axis &other) const
{
	return enabled == other.enabled
		&& range == other.range
		&& step == other.step
		&& unit == other.unit
		&& title == other.title;
}

double Axis::origo() const
{
	if (range.size() == 0) return 0;
	return -range.getMin() / range.size();
}

Axis interpolate(const Axis &op0, const Axis &op1, double factor)
{
	Axis res;
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

	//todo: interpolate unit
	res.unit = op1.unit;
	res.title = interpolate(op0.title, op1.title, factor);

	return res;
}

DiscreteAxis::DiscreteAxis()
{
	enabled = false;
}

bool DiscreteAxis::add(const Data::MultiDim::SliceIndex &index,
					   double value,
					   Math::Range<double> &range,
					   double enabled)
{
	if (enabled <= 0) return false;

	this->enabled = true;

	auto it = values.find(index);
	if (it == values.end())
	{
		values.insert({index,
		    Item{true, true, range, value, Gfx::Color(), std::string(), enabled}});
		return true;
	} else {
		it->second.range.include(range);
		it->second.weight = std::max(it->second.weight, enabled);
		return false;
	}
}

bool DiscreteAxis::operator==(const DiscreteAxis &other) const
{
	return enabled == other.enabled
		&& values == other.values;
}

void DiscreteAxis::setLabels(const Data::DataCube &data, const Data::DataTable &table)
{
	Values::iterator it;
	for (it = values.begin();
		 it != values.end();
		 ++it)
	{
		auto colIndex = data.getSeriesByDim(it->first.dimIndex).getColIndex();
		const auto &discreteValues = table.getInfo(colIndex).discreteValues();
		if (it->first.index < discreteValues.size())
			it->second.label = discreteValues[it->first.index];
		else
			it->second.label = "NA";
	}
}

DiscreteAxis interpolate(const DiscreteAxis &op0, const DiscreteAxis &op1, double factor)
{
	DiscreteAxis res;
	res.title = interpolate(op0.title, op1.title, factor);

	DiscreteAxis::Values::const_iterator it;
	for (it = op0.values.cbegin(); it != op0.values.cend(); ++it)
	{
		res.enabled = true;
		res.values.insert({it->first,
						  DiscreteAxis::Item{
							  true, false,
							  it->second.range,
							  it->second.value,
							  it->second.color,
							  it->second.label,
							  it->second.weight * (1-factor)
						  }});
	}

	for (it = op1.values.cbegin(); it != op1.values.cend(); ++it)
	{
		res.enabled = true;
		auto resIt = res.values.find(it->first);
		if (resIt == res.values.cend())
		{
			res.values.insert({it->first,
							  DiscreteAxis::Item{
								  false, true,
								  it->second.range,
								  it->second.value,
								  it->second.color,
								  it->second.label,
								  it->second.weight * factor
							  }});
		}
		else
		{
			resIt->second.end = true;

			resIt->second.range =
			    Math::interpolate(resIt->second.range,
			        it->second.range,
			        factor);

			resIt->second.color =
			    Math::interpolate(resIt->second.color,
					it->second.color, factor);

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
