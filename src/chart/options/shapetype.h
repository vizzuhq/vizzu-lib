#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <array>
#include <string>

#include "base/math/fuzzybool.h"

namespace Vizzu
{
namespace Diag
{

class ShapeType
{
public:

	enum Type : uint32_t {
		Rectangle, Circle, Area, Line,
		size
	};

	typedef std::array<Math::FuzzyBool, (size_t)Type::size> ShapeFactors;

	ShapeType(ShapeType::Type type = ShapeType::Rectangle)
	{
		for (auto &factor: shapeFactors) factor = Math::FuzzyBool(false);
		shapeFactors[(size_t)type] = Math::FuzzyBool(true);
	}

	explicit operator ShapeType::Type() const
	{
		//todo: internal error if more then one true in []
		for (auto i = 0u; i < shapeFactors.size(); i++)
			if (shapeFactors[i] == 1.0) return Type(i);
		throw std::logic_error("internal error, mixed shape type");
	}

	ShapeType::Type type() const
	{
		return (ShapeType::Type)*this;
	}

	bool operator==(ShapeType::Type type) const
	{
		//todo: internal error if more then one true in []
		return (bool)shapeFactors[(size_t)type];
	}

	bool operator==(const ShapeType &other) const
	{
		return shapeFactors == other.shapeFactors;
	}

	bool operator!=(const ShapeType &other) const
	{
		for (auto i = 0u; i < shapeFactors.size(); i++)
			if (shapeFactors[i] != other.shapeFactors[i])
				return true;
		return false;
	}

	ShapeType operator*(double factor) const
	{
		auto res = *this;
		for (auto &value : res.shapeFactors)
			value = value * factor;
		return res;
	}

	ShapeType operator+(const ShapeType &st) const
	{
		ShapeType res;
		for (auto i = 0u; i < res.shapeFactors.size(); i++)
			res.shapeFactors[i] =
				shapeFactors[i] + st.shapeFactors[i];
		return res;
	}

	const Math::FuzzyBool &getFactor(Type type) const
	{
		return shapeFactors[type];
	}

	const ShapeFactors &getFactors() const
	{
		return shapeFactors;
	}

private:
	ShapeFactors shapeFactors;
};

std::string toString(ShapeType::Type type);
bool canOverlap(ShapeType::Type type);
bool isConnecting(ShapeType::Type type);

}
}

#endif
