#ifndef GFX_COLORCONVCANVAS
#define GFX_COLORCONVCANVAS

#include <optional>

#include "canvas.h"

namespace Gfx
{

template<class Parent = ICanvas>
class ColorConversionCanvas : public Parent
{
public:
	using Parent::Parent;

	void setColorTransform(const ColorTransform &transform) override
	{
		this->transform = transform;
	}

	void resetColorTransform() override
	{
		transform.reset();
	}

	void setBrushColor(const Gfx::Color &color) override
	{
		Parent::setBrushColor(convert(color));
	}

	void setLineColor(const Gfx::Color &color) override
	{
		Parent::setLineColor(convert(color));
	}

	void setTextColor(const Gfx::Color &color) override
	{
		Parent::setTextColor(convert(color));
	}

	void setBrushGradient(const Geom::Line &line, const Gfx::ColorGradient &gradient) override
	{
		auto convGradient = gradient;
		for (auto &stop : convGradient.stops) stop.value = convert(stop.value);
		Parent::setBrushGradient(line, convGradient);
	}

private:
	std::optional<ColorTransform> transform;

	Gfx::Color convert(const Gfx::Color &color) const {
		return transform ? (*transform)(color) : color;
	}
};

}

#endif
