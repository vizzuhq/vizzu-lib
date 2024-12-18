#include "morph.h"

#include <memory>
#include <stdexcept>

#include "base/anim/interpolated.h"
#include "base/math/interpolation.h"
#include "chart/generator/axis.h"
#include "chart/generator/guides.h"
#include "chart/generator/plot.h"
#include "chart/options/channel.h"
#include "chart/options/shapetype.h"

#include "options.h"

namespace Vizzu::Anim::Morph
{

using Math::Niebloid::interpolate;

AbstractMorph::AbstractMorph(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual) :
    source(source),
    target(target),
    actual(actual)
{}

std::unique_ptr<AbstractMorph> AbstractMorph::create(
    SectionId sectionId,
    const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual)
{
	switch (sectionId) {
	case SectionId::color:
		return std::make_unique<Color>(source, target, actual);
	case SectionId::show:
		return std::make_unique<Show>(source, target, actual);
	case SectionId::hide:
		return std::make_unique<Hide>(source, target, actual);
	case SectionId::x:
		return std::make_unique<Horizontal>(source, target, actual);
	case SectionId::y:
		return std::make_unique<Vertical>(source, target, actual);
	case SectionId::geometry:
		return std::make_unique<Shape>(source, target, actual);
	case SectionId::coordSystem:
		return std::make_unique<CoordinateSystem>(source,
		    target,
		    actual);
	case SectionId::connection:
		return std::make_unique<Connection>(source, target, actual);
	default: throw std::logic_error("invalid animation section");
	}
}

void AbstractMorph::transform(double factor)
{
	transform(source, target, actual, factor);

	transform(*source.getOptions(),
	    *target.getOptions(),
	    *actual.getOptions(),
	    factor);

	for (auto i = 0U; i < source.getMarkers().size(); ++i) {
		transform(source.getMarkers()[i],
		    target.getMarkers()[i],
		    actual.getMarkers()[i],
		    factor);
	}
}

void CoordinateSystem::transform(const Gen::Options &source,
    const Gen::Options &target,
    Gen::Options &actual,
    double factor) const
{
	actual.coordSystem =
	    interpolate(source.coordSystem, target.coordSystem, factor);
	actual.angle = interpolate(source.angle, target.angle, factor);
}

void Show::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	if (!source.enabled && target.enabled)
		actual.enabled =
		    interpolate(source.enabled, target.enabled, factor);
}

void Hide::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	if (source.enabled && !target.enabled)
		actual.enabled =
		    interpolate(source.enabled, target.enabled, factor);
}

void Shape::transform(const Gen::Options &source,
    const Gen::Options &target,
    Gen::Options &actual,
    double factor) const
{
	actual.geometry =
	    interpolate(source.geometry, target.geometry, factor);
}

void Horizontal::transform(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    double factor) const
{
	actual.axises.at(Gen::AxisId::x) =
	    interpolate(source.axises.at(Gen::AxisId::x),
	        target.axises.at(Gen::AxisId::x),
	        factor);

	actual.keepAspectRatio = interpolate(source.keepAspectRatio,
	    target.keepAspectRatio,
	    factor);

	actual.guides.x =
	    interpolate(source.guides.x, target.guides.x, factor);
}

void Horizontal::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	actual.position.x =
	    interpolate(source.position.x, target.position.x, factor);
	actual.size.x = interpolate(source.size.x, target.size.x, factor);
	actual.spacing.x =
	    interpolate(source.spacing.x, target.spacing.x, factor);
}

void Connection::transform(const Gen::Options &source,
    const Gen::Options &target,
    Gen::Options &actual,
    double factor) const
{
	auto sourceIsConnecting = isConnecting(source.geometry.get());
	auto targetIsConnecting = isConnecting(target.geometry.get());

	if (sourceIsConnecting && !targetIsConnecting) {
		actual.orientation = source.orientation;
	}
	else if (!sourceIsConnecting && targetIsConnecting) {
		actual.orientation = target.orientation;
	}
	else {
		actual.orientation = interpolate(source.orientation,
		    target.orientation,
		    factor);
	}
}

void Connection::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	actual.prevMainMarker = interpolate(source.prevMainMarker,
	    target.prevMainMarker,
	    factor);

	actual.polarConnection = interpolate(source.polarConnection,
	    target.polarConnection,
	    factor);
}

void Vertical::transform(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    double factor) const
{
	actual.axises.at(Gen::AxisId::y) =
	    interpolate(source.axises.at(Gen::AxisId::y),
	        target.axises.at(Gen::AxisId::y),
	        factor);

	actual.axises.addLegendInterpolation(
	    actual.getOptions()->legend.factor(Gen::LegendId::size),
	    Gen::LegendId::size,
	    source.axises.at(Gen::LegendId::size),
	    target.axises.at(Gen::LegendId::size),
	    factor);

	actual.guides.y =
	    interpolate(source.guides.y, target.guides.y, factor);

	actual.axises.label =
	    interpolate(source.axises.label, target.axises.label, factor);
}

void Vertical::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	actual.position.y =
	    interpolate(source.position.y, target.position.y, factor);
	actual.size.y = interpolate(source.size.y, target.size.y, factor);
	actual.spacing.y =
	    interpolate(source.spacing.y, target.spacing.y, factor);
	actual.sizeFactor =
	    interpolate(source.sizeFactor, target.sizeFactor, factor);
	actual.label = interpolate(source.label, target.label, factor);
}

void Morph::Color::transform(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    double factor) const
{
	actual.axises.addLegendInterpolation(
	    actual.getOptions()->legend.factor(Gen::LegendId::color),
	    Gen::LegendId::color,
	    source.axises.at(Gen::LegendId::color),
	    target.axises.at(Gen::LegendId::color),
	    factor);

	actual.axises.addLegendInterpolation(
	    actual.getOptions()->legend.factor(Gen::LegendId::lightness),
	    Gen::LegendId::lightness,
	    source.axises.at(Gen::LegendId::lightness),
	    target.axises.at(Gen::LegendId::lightness),
	    factor);
}

void Morph::Color::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	actual.colorBase =
	    interpolate(source.colorBase, target.colorBase, factor);
}

}