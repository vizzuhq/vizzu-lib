#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <array>
#include <optional>
#include <string>

#include "base/math/fuzzybool.h"

namespace Vizzu
{
namespace Gen
{

class ShapeType
{
public:
	enum Type : uint32_t { Rectangle, Circle, Area, Line, size };

	typedef std::array<Math::FuzzyBool, (size_t)Type::size>
	    ShapeFactors;

	ShapeType(ShapeType::Type type = ShapeType::Rectangle)
	{
		for (auto &factor : shapeFactors)
			factor = Math::FuzzyBool(false);
		shapeFactors[(size_t)type] = Math::FuzzyBool(true);
	}

	std::optional<ShapeType::Type> typeIf() const
	{
		std::optional<ShapeType::Type> res;
		for (auto i = 0u; i < shapeFactors.size(); i++)
			if (shapeFactors[i] == 1.0) {
				if (res) [[unlikely]]
					throw std::logic_error(
					    "internal error, multi shape type");
				else
					res.emplace(static_cast<Type>(i));
			}
		return res;
	}

	explicit operator ShapeType::Type() const
	{
		if (auto res = typeIf()) [[likely]]
			return *res;
		throw std::logic_error("internal error, mixed shape type");
	}

	ShapeType::Type type() const { return (ShapeType::Type) * this; }

	bool operator==(ShapeType::Type type) const
	{
		return typeIf() == type;
	}

	bool operator==(const ShapeType &other) const = default;
	bool operator!=(const ShapeType &other) const = default;

	ShapeType operator*(double factor) const
	{
		auto res = *this;
		for (auto &value : res.shapeFactors) value = value * factor;
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
