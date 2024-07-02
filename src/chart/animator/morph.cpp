#include "morph.h"

#include "base/math/interpolation.h"
#include "chart/generator/plot.h"

namespace Vizzu::Anim::Morph
{

using Math::interpolate;

template <class T>
::Anim::Interpolated<T> safe_interpolate(
    const ::Anim::Interpolated<T> &source,
    const ::Anim::Interpolated<T> &target,
    double factor)
{
	::Anim::Interpolated<T> actual =
	    interpolate(source, target, factor);
	if (factor == 1.0) {
		actual.values[0].value = source.values[0].value;
		if (source.values[0].value == target.values[0].value) {
			actual.values[0].weight = actual.values[1].weight;
			actual.has_second = false;
		}
	}

	return actual;
}

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
	actual.coordSystem = safe_interpolate(source.coordSystem,
	    target.coordSystem,
	    factor);
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
	    safe_interpolate(source.geometry, target.geometry, factor);
}

void Horizontal::transform(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    double factor) const
{
	actual.axises.at(Gen::ChannelId::x).common =
	    interpolate(source.axises.at(Gen::ChannelId::x).common,
	        target.axises.at(Gen::ChannelId::x).common,
	        factor);

	actual.axises.at(Gen::ChannelId::x).measure =
	    interpolate(source.axises.at(Gen::ChannelId::x).measure,
	        target.axises.at(Gen::ChannelId::x).measure,
	        factor);

	actual.axises.at(Gen::ChannelId::x).dimension =
	    interpolate(source.axises.at(Gen::ChannelId::x).dimension,
	        target.axises.at(Gen::ChannelId::x).dimension,
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
	auto sourceIsConnecting = isConnecting(source.geometry.get());
	auto targetIsConnecting = isConnecting(target.geometry.get());

	if (sourceIsConnecting && !targetIsConnecting) {
		actual.orientation = source.orientation;
	}
	else if (!sourceIsConnecting && targetIsConnecting) {
		actual.orientation = target.orientation;
	}
	else {
		actual.orientation = safe_interpolate(source.orientation,
		    target.orientation,
		    factor);
	}
}

void Connection::transform(const Marker &source,
    const Marker &target,
    Marker &actual,
    double factor) const
{
	actual.prevMainMarker = safe_interpolate(source.prevMainMarker,
	    target.prevMainMarker,
	    factor);

	actual.polarConnection = safe_interpolate(source.polarConnection,
	    target.polarConnection,
	    factor);
}

void Vertical::transform(const Gen::Plot &source,
    const Gen::Plot &target,
    Gen::Plot &actual,
    double factor) const
{
	actual.axises.at(Gen::ChannelId::y).common =
	    interpolate(source.axises.at(Gen::ChannelId::y).common,
	        target.axises.at(Gen::ChannelId::y).common,
	        factor);

	actual.axises.at(Gen::ChannelId::y).measure =
	    interpolate(source.axises.at(Gen::ChannelId::y).measure,
	        target.axises.at(Gen::ChannelId::y).measure,
	        factor);

	actual.axises.at(Gen::ChannelId::y).dimension =
	    interpolate(source.axises.at(Gen::ChannelId::y).dimension,
	        target.axises.at(Gen::ChannelId::y).dimension,
	        factor);

	actual.axises.at(Gen::ChannelId::size).common =
	    interpolate(source.axises.at(Gen::ChannelId::size).common,
	        target.axises.at(Gen::ChannelId::size).common,
	        factor);

	actual.axises.at(Gen::ChannelId::size).measure =
	    interpolate(source.axises.at(Gen::ChannelId::size).measure,
	        target.axises.at(Gen::ChannelId::size).measure,
	        factor);

	actual.axises.at(Gen::ChannelId::size).dimension =
	    interpolate(source.axises.at(Gen::ChannelId::size).dimension,
	        target.axises.at(Gen::ChannelId::size).dimension,
	        factor);

	actual.guides.y =
	    interpolate(source.guides.y, target.guides.y, factor);

	actual.axises.at(Gen::ChannelId::label).measure =
	    interpolate(source.axises.at(Gen::ChannelId::label).measure,
	        target.axises.at(Gen::ChannelId::label).measure,
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
	actual.axises.at(Gen::ChannelId::color).common =
	    interpolate(source.axises.at(Gen::ChannelId::color).common,
	        target.axises.at(Gen::ChannelId::color).common,
	        factor);

	actual.axises.at(Gen::ChannelId::color).measure =
	    interpolate(source.axises.at(Gen::ChannelId::color).measure,
	        target.axises.at(Gen::ChannelId::color).measure,
	        factor);

	actual.axises.at(Gen::ChannelId::color).dimension =
	    interpolate(source.axises.at(Gen::ChannelId::color).dimension,
	        target.axises.at(Gen::ChannelId::color).dimension,
	        factor);

	actual.axises.at(Gen::ChannelId::lightness).common = interpolate(
	    source.axises.at(Gen::ChannelId::lightness).common,
	    target.axises.at(Gen::ChannelId::lightness).common,
	    factor);

	actual.axises.at(Gen::ChannelId::lightness).measure = interpolate(
	    source.axises.at(Gen::ChannelId::lightness).measure,
	    target.axises.at(Gen::ChannelId::lightness).measure,
	    factor);

	actual.axises.at(Gen::ChannelId::lightness).dimension =
	    interpolate(
	        source.axises.at(Gen::ChannelId::lightness).dimension,
	        target.axises.at(Gen::ChannelId::lightness).dimension,
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