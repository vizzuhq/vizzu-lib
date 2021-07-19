#include "axis.h"

#include "base/io/log.h"
#include "base/math/interpolation.h"
#include "base/math/renard.h"

namespace Vizzu::Diag
{

Geom::Point Axises::origo() const {
	return Geom::Point(
		at(Scale::Type::X).origo(),
		at(Scale::Y).origo());
}

Axis::Axis()
{
	enabled = false;
	range = Math::Range<double>(0,1);
	step = 1.0;
}

Axis::Axis(Math::Range<double> interval, std::string title, std::string unit) :
    range(interval),
	title(std::move(title)),
    unit(std::move(unit))
{
	enabled = true;
	step = Math::Renard::R5().ceil(range.size() / 5.0);
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
	using D = double;

	Axis res;
	res.enabled = Math::interpolate(op0.enabled, op1.enabled, factor);

	auto range0 = Math::interpolate(op1.range, op0.range, (D)op0.enabled);
	auto range1 = Math::interpolate(op0.range, op1.range, (D)op1.enabled);

	auto step0 = Math::interpolate(op1.step, op0.step, (D)op0.enabled);
	auto step1 = Math::interpolate(op0.step, op1.step, (D)op1.enabled);

	res.range = Math::interpolate(range0, range1, factor);
	res.step = Math::interpolate(step0, step1, factor);
	//todo: interpolate unit
	res.unit = op1.unit;
	res.title = Math::interpolate(op0.title, op1.title, factor);

	return res;
}

DiscreteAxis::DiscreteAxis()
{
	enabled = false;
}

void DiscreteAxis::add(const Data::MultiDim::SliceIndex &index,
					   double value,
					   Math::Range<double> &range,
					   double enabled)
{
	if (enabled <= 0) return;

	this->enabled = true;

	auto it = values.find(index);
	if (it == values.end())
	{
		values.insert({index,
		    Item{range, value, Gfx::Color(), nullptr, enabled}});
	} else {
		it->second.range.include(range);
		it->second.weight = std::max(it->second.weight, enabled);
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
			it->second.label = discreteValues[it->first.index].c_str();
		else
			it->second.label = "NA";
	}
}

DiscreteAxis interpolate(const DiscreteAxis &op0, const DiscreteAxis &op1, double factor)
{
	DiscreteAxis res;
	res.title = Math::interpolate(op0.title, op1.title, factor);

	DiscreteAxis::Values::const_iterator it;
	for (it = op0.values.cbegin(); it != op0.values.cend(); ++it)
	{
		res.enabled = true;
		res.values.insert({it->first,
						  DiscreteAxis::Item{
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
								  it->second.range,
								  it->second.value,
								  it->second.color,
								  it->second.label,
								  it->second.weight * factor
							  }});
		}
		else
		{
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
