#include "morph.h"

#include "base/math/interpolation.h"
#include "chart/generator/plot.h"

namespace Vizzu::Anim::Morph
{

using Math::interpolate;

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
	    *actual.options,
	    factor);

	for (auto i = 0U; i < source.getMarkers().size(); ++i) {
		transform(source.getMarkers()[i],
		    target.getMarkers()[i],
		    actual.markers[i],
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
	actual.commonAxises.at(Gen::ChannelId::x) =
	    interpolate(source.commonAxises.at(Gen::ChannelId::x),
	        target.commonAxises.at(Gen::ChannelId::x),
	        factor);

	actual.measureAxises.at(Gen::ChannelId::x) =
	    interpolate(source.measureAxises.at(Gen::ChannelId::x),
	        target.measureAxises.at(Gen::ChannelId::x),
	        factor);

	actual.dimensionAxises.at(Gen::ChannelId::x) =
	    interpolate(source.dimensionAxises.at(Gen::ChannelId::x),
	        target.dimensionAxises.at(Gen::ChannelId::x),
	        factor);

	actual.keepAspectRatio = interpolate(source.keepAspectRatio,
	    target.keepAspectRatio,
	    factor);

	actual.anyAxisSet =
	    interpolate(source.anyAxisSet, target.anyAxisSet, factor);

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
	auto sourceIsConnecting =
	    Vizzu::Gen::isConnecting(source.geometry.get());
	auto targetIsConnecting =
	    Vizzu::Gen::isConnecting(target.geometry.get());

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
	actual.prevMainMarkerIdx = interpolate(source.prevMainMarkerIdx,
	    target.prevMainMarkerIdx,
	    factor);

	actual.mainId = interpolate(source.mainId, target.mainId, factor);
}

void Vertical::transform(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    double factor) const
{
	actual.commonAxises.at(Gen::ChannelId::y) =
	    interpolate(source.commonAxises.at(Gen::ChannelId::y),
	        target.commonAxises.at(Gen::ChannelId::y),
	        factor);

	actual.measureAxises.at(Gen::ChannelId::y) =
	    interpolate(source.measureAxises.at(Gen::ChannelId::y),
	        target.measureAxises.at(Gen::ChannelId::y),
	        factor);

	actual.dimensionAxises.at(Gen::ChannelId::y) =
	    interpolate(source.dimensionAxises.at(Gen::ChannelId::y),
	        target.dimensionAxises.at(Gen::ChannelId::y),
	        factor);

	actual.commonAxises.at(Gen::ChannelId::size) =
	    interpolate(source.commonAxises.at(Gen::ChannelId::size),
	        target.commonAxises.at(Gen::ChannelId::size),
	        factor);

	actual.measureAxises.at(Gen::ChannelId::size) =
	    interpolate(source.measureAxises.at(Gen::ChannelId::size),
	        target.measureAxises.at(Gen::ChannelId::size),
	        factor);

	actual.dimensionAxises.at(Gen::ChannelId::size) =
	    interpolate(source.dimensionAxises.at(Gen::ChannelId::size),
	        target.dimensionAxises.at(Gen::ChannelId::size),
	        factor);

	actual.guides.y =
	    interpolate(source.guides.y, target.guides.y, factor);

	actual.measureAxises.at(Gen::ChannelId::label) =
	    interpolate(source.measureAxises.at(Gen::ChannelId::label),
	        target.measureAxises.at(Gen::ChannelId::label),
	        factor);
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
	actual.commonAxises.at(Gen::ChannelId::color) =
	    interpolate(source.commonAxises.at(Gen::ChannelId::color),
	        target.commonAxises.at(Gen::ChannelId::color),
	        factor);

	actual.measureAxises.at(Gen::ChannelId::color) =
	    interpolate(source.measureAxises.at(Gen::ChannelId::color),
	        target.measureAxises.at(Gen::ChannelId::color),
	        factor);

	actual.dimensionAxises.at(Gen::ChannelId::color) =
	    interpolate(source.dimensionAxises.at(Gen::ChannelId::color),
	        target.dimensionAxises.at(Gen::ChannelId::color),
	        factor);

	actual.commonAxises.at(Gen::ChannelId::lightness) =
	    interpolate(source.commonAxises.at(Gen::ChannelId::lightness),
	        target.commonAxises.at(Gen::ChannelId::lightness),
	        factor);

	actual.measureAxises.at(Gen::ChannelId::lightness) = interpolate(
	    source.measureAxises.at(Gen::ChannelId::lightness),
	    target.measureAxises.at(Gen::ChannelId::lightness),
	    factor);

	actual.dimensionAxises.at(Gen::ChannelId::lightness) =
	    interpolate(
	        source.dimensionAxises.at(Gen::ChannelId::lightness),
	        target.dimensionAxises.at(Gen::ChannelId::lightness),
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