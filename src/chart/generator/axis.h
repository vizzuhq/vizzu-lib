#ifndef AXIS_H
#define AXIS_H

#include <map>

#include "base/anim/interpolated.h"
#include "base/gfx/color.h"
#include "base/geom/point.h"
#include "base/math/fuzzybool.h"
#include "base/math/interpolation.h"
#include "base/math/range.h"
#include "chart/options/scale.h"
#include "data/datacube/datacube.h"
#include "data/multidim/multidimindex.h"
#include "data/table/datatable.h"

namespace Vizzu
{
namespace Diag
{

template <typename Type>
struct AbstractAxises
{
	std::array<Type, Scale::Type::id_size> axises;

	const Type &at(Scale::Type scaleType) const
	{
		return axises.at(scaleType);
	}

	Type &at(Scale::Type scaleType) { return axises.at(scaleType); }

	const Type &other(Scale::Type scaleType) const
	{
		return scaleType == Scale::Type::X ? axises.at(Scale::Type::Y) :
		       scaleType == Scale::Type::Y ? axises.at(Scale::Type::X) :
			   throw std::logic_error("not an axis scale");
	}

	bool operator==(const AbstractAxises<Type> &other) const
	{
		for (auto i = 0; i < (int)Scale::Type::id_size; i++)
		{
			auto id = Scale::Type(i);
			if (axises[id] != other.axises[id])
				return false;
		}
		return true;
	}
};

struct Axis
{
	Math::FuzzyBool enabled;
	Math::Range<double> range;
	::Anim::String title;
	std::string unit;
	double step;
	Axis();
	Axis(Math::Range<double> interval, std::string title, std::string unit);
	bool operator==(const Axis &other) const;
	double origo() const;
};

Axis interpolate(const Axis &op0,
    const Axis &op1,
    double factor);

struct Axises: public AbstractAxises<Axis>
{
	Geom::Point origo() const;
};

struct DiscreteAxis
{
	friend DiscreteAxis interpolate(const DiscreteAxis &op0,
									const DiscreteAxis &op1,
									double factor);
public:
	struct Item {
		Math::Range<double> range;
		double value;
		Gfx::Color color;
		const char *label;
		double weight;
		bool operator==(const Item &other) const
		{ return range == other.range; }
	};
	typedef std::map<Data::MultiDim::SliceIndex, Item> Values;

	Math::FuzzyBool enabled;
	::Anim::String title;

	DiscreteAxis();
	bool add(const Data::MultiDim::SliceIndex &index,
		double value,
	    Math::Range<double> &range,
	    double enabled);
	bool operator==(const DiscreteAxis &other) const;

	Values::iterator begin() { return values.begin(); };
	Values::iterator end() { return values.end(); }
	Values::const_iterator begin() const { return values.cbegin(); };
	Values::const_iterator end() const { return values.cend(); }
	void setLabels(const Data::DataCube &data, const Data::DataTable &table);

private:
	Values values;
};

DiscreteAxis interpolate(const DiscreteAxis &op0, const DiscreteAxis &op1, double factor);

typedef AbstractAxises<DiscreteAxis> DiscreteAxises;

}
}

#endif
