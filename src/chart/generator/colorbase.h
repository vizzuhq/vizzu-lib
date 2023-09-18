#ifndef COLORBASE_H
#define COLORBASE_H

#include <cstdint>
#include <variant>

namespace Vizzu::Gen
{

struct ColorBase
{
	[[nodiscard]] double getLightness() const { return lightness; }

	[[nodiscard]] uint32_t getIndex() const
	{
		return std::get<uint32_t>(indexOrPos);
	}

	[[nodiscard]] double getPos() const
	{
		return std::get<double>(indexOrPos);
	}

	void setLightness(double value) { lightness = value; }

	void setPos(double pos) { indexOrPos = pos; }

	[[nodiscard]] bool isDiscrete() const
	{
		return std::holds_alternative<uint32_t>(indexOrPos);
	}

	ColorBase() = default;

	ColorBase(uint32_t index, double lightness) :
	    lightness(lightness),
	    indexOrPos(index)
	{}

	ColorBase(double pos, double lightness) :
	    lightness(lightness),
	    indexOrPos(pos)
	{}

	bool operator==(const ColorBase &other) const = default;

private:
	double lightness{0};
	std::variant<uint32_t, double> indexOrPos{0U};
};

}

#endif
