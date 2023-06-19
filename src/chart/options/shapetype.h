#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <array>
#include <string>

#include "base/math/fuzzybool.h"
#include "base/refl/auto_enum.h"

namespace Vizzu
{
namespace Gen
{

class ShapeType
{
public:
	enum Type : uint32_t { Rectangle, Circle, Area, Line };

	typedef Refl::EnumArray<Type, Math::FuzzyBool> ShapeFactors;

	ShapeType(ShapeType::Type type = ShapeType::Rectangle)
	{
		for (auto &factor : shapeFactors)
			factor = Math::FuzzyBool(false);
		shapeFactors[type] = Math::FuzzyBool(true);
	}

	explicit operator ShapeType::Type() const
	{
		for (auto i = 0u; i < shapeFactors.size(); i++)
			if (shapeFactors[static_cast<Type>(i)] == 1.0)
				return Type(i);
		throw std::logic_error("internal error, mixed shape type");
	}

	ShapeType::Type type() const { return (ShapeType::Type) * this; }

	bool operator==(ShapeType::Type type) const
	{
		return (bool)shapeFactors[type];
	}

	bool operator==(const ShapeType &other) const
	{
		return shapeFactors == other.shapeFactors;
	}

	bool operator!=(const ShapeType &other) const
	{
		for (auto i = 0u; i < shapeFactors.size(); i++)
			if (auto t = static_cast<Type>(i);
			    shapeFactors[t] != other.shapeFactors[t])
				return true;
		return false;
	}

	ShapeType operator*(double factor) const
	{
		auto res = *this;
		for (auto &value : res.shapeFactors) value = value * factor;
		return res;
	}

	ShapeType operator+(const ShapeType &st) const
	{
		ShapeType res;
		for (auto i = 0u; i < res.shapeFactors.size(); i++) {
			auto t = static_cast<Type>(i);
			res.shapeFactors[t] =
			    shapeFactors[t] + st.shapeFactors[t];
		}
		return res;
	}

	const Math::FuzzyBool &getFactor(Type type) const
	{
		return shapeFactors[type];
	}

	const ShapeFactors &getFactors() const { return shapeFactors; }

private:
	ShapeFactors shapeFactors;
};

std::string toString(ShapeType::Type type);
bool canOverlap(ShapeType::Type type);
bool isConnecting(ShapeType::Type type);

}
}

#endif
