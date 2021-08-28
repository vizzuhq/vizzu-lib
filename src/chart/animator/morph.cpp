#include "morph.h"

#include "base/io/log.h"
#include "base/math/interpolation.h"

using namespace Vizzu;
using namespace Vizzu::Base;
using namespace Vizzu::Anim;
using namespace Vizzu::Diag;
using namespace Vizzu::Anim::Morph;
using namespace Math;

AbstractMorph::AbstractMorph(
	const Diagram &source,
    const Diagram &target,
    Diagram &actual) :
    source(source),
    target(target),
    actual(actual)
{}

std::unique_ptr<AbstractMorph> AbstractMorph::create(SectionId sectionId,
	const Diagram &source,
    const Diagram &target,
    Diagram &actual)
{
	switch(sectionId) {
	case SectionId::EnumType::color: 
		return std::make_unique<Color>(source, target, actual);
	case SectionId::EnumType::show:
		return std::make_unique<Show>(source, target, actual);
	case SectionId::EnumType::hide:
		return std::make_unique<Hide>(source, target, actual);
	case SectionId::EnumType::x: 
		return std::make_unique<Horizontal>(source, target, actual);
	case SectionId::EnumType::y: 
		return std::make_unique<Vertical>(source, target, actual);
	case SectionId::EnumType::geometry: 
		return std::make_unique<Shape>(source, target, actual);
	case SectionId::EnumType::coordSystem: 
		return std::make_unique<CoordinateSystem>
			(source, target, actual);
	default: throw std::logic_error("invalid animation section");
	}
}

void AbstractMorph::transform(double factor)
{
	transform(source, target, actual, factor);

	transform(*source.getOptions(), *target.getOptions(),
		*actual.options, factor);

	for (auto i = 0u; i < source.getMarkers().size(); i++)
	{
		transform(source.getMarkers()[i], target.getMarkers()[i],
			actual.markers[i], factor);
	}
}

void CoordinateSystem::transform(const Diag::Options &source,
							const Diag::Options &target,
							Diag::Options &actual,
							double factor) const
{
	actual.polar.set(interpolate(source.polar.get(), target.polar.get(), factor));
	actual.angle.set(interpolate(source.angle.get(), target.angle.get(), factor));
}

void Show::transform(const Marker &source,
				   const Marker &target,
				   Marker &actual,
				   double factor) const
{
	if (!source.enabled && target.enabled)
		actual.enabled = interpolate(source.enabled, target.enabled, factor);
}

void Hide::transform(const Marker &source,
				   const Marker &target,
				   Marker &actual,
				   double factor) const
{
	if (source.enabled && !target.enabled)
		actual.enabled = interpolate(source.enabled, target.enabled, factor);
}

void Shape::transform(const Diag::Options &source,
				  const Diag::Options &target,
				  Diag::Options &actual,
				  double factor) const
{
	actual.shapeType.set(interpolate(source.shapeType.get(),
										target.shapeType.get(),
										factor));
}

void Horizontal::transform(const Diagram &source,
					  const Diagram &target,
					  Diagram &actual,
					  double factor) const
{
	actual.axises.at(Diag::ScaleId::x) =
	    interpolate(source.axises.at(Diag::ScaleId::x),
	        target.axises.at(Diag::ScaleId::x),
	        factor);

	actual.discreteAxises.at(Diag::ScaleId::x) =
	    interpolate(source.discreteAxises.at(Diag::ScaleId::x),
	        target.discreteAxises.at(Diag::ScaleId::x),
	        factor);

	actual.keepAspectRatio =
		interpolate(source.keepAspectRatio, target.keepAspectRatio, factor);

	actual.anyAxisSet =
		interpolate(source.anyAxisSet, target.anyAxisSet, factor);
}

void Horizontal::transform(const Diag::Options &source,
				  const Diag::Options &target,
				  Diag::Options &actual,
				  double factor) const
{
	auto sourceIsConnecting = Vizzu::Diag::isConnecting(source.shapeType.get().type());
	auto targetIsConnecting = Vizzu::Diag::isConnecting(target.shapeType.get().type());

	if (sourceIsConnecting && !targetIsConnecting)
	{
		actual.horizontal.set(source.horizontal.get());
	}
	else if (!sourceIsConnecting && targetIsConnecting)
	{
		actual.horizontal.set(target.horizontal.get());
	}
	else
	{
		actual.horizontal.set(interpolate(source.horizontal.get(),
										  target.horizontal.get(),
										  factor));
	}
}

void Horizontal::transform(const Marker &source,
					   const Marker &target,
					   Marker &actual,
					   double factor) const
{
	actual.position.x = interpolate(source.position.x, target.position.x, factor);
	actual.size.x = interpolate(source.size.x, target.size.x, factor);
	actual.spacing.x = interpolate(source.spacing.x, target.spacing.x, factor);
}

void Vertical::transform(const Diagram &source,
					  const Diagram &target,
					  Diagram &actual,
					  double factor) const
{
	actual.axises.at(Diag::ScaleId::y) =
	    interpolate(source.axises.at(Diag::ScaleId::y),
	        target.axises.at(Diag::ScaleId::y),
	        factor);

	actual.discreteAxises.at(Diag::ScaleId::y) =
	    interpolate(source.discreteAxises.at(Diag::ScaleId::y),
	        target.discreteAxises.at(Diag::ScaleId::y),
	        factor);

	actual.axises.at(Diag::ScaleId::size) =
	    interpolate(source.axises.at(Diag::ScaleId::size),
	        target.axises.at(Diag::ScaleId::size),
	        factor);

	actual.discreteAxises.at(Diag::ScaleId::size) =
	    interpolate(source.discreteAxises.at(Diag::ScaleId::size),
	        target.discreteAxises.at(Diag::ScaleId::size),
	        factor);
}

void Vertical::transform(const Marker &source,
					 const Marker &target,
					 Marker &actual,
					 double factor) const
{
	actual.position.y = interpolate(source.position.y, target.position.y, factor);
	actual.size.y = interpolate(source.size.y, target.size.y, factor);
	actual.spacing.y = interpolate(source.spacing.y, target.spacing.y, factor);
	actual.sizeFactor = interpolate(source.sizeFactor, target.sizeFactor, factor);
	actual.label = interpolate(source.label, target.label, factor);
}

void Morph::Color::transform(const Diagram &source,
				 const Diagram &target,
				 Diagram &actual,
				 double factor) const
{
	actual.anySelected =
		interpolate(source.anySelected, target.anySelected, factor);

	actual.axises.at(Diag::ScaleId::color) =
	    interpolate(source.axises.at(Diag::ScaleId::color),
	        target.axises.at(Diag::ScaleId::color),
	        factor);

	actual.discreteAxises.at(Diag::ScaleId::color) =
	    interpolate(source.discreteAxises.at(Diag::ScaleId::color),
	        target.discreteAxises.at(Diag::ScaleId::color),
	        factor);

	actual.axises.at(Diag::ScaleId::lightness) =
	    interpolate(source.axises.at(Diag::ScaleId::lightness),
	        target.axises.at(Diag::ScaleId::lightness),
	        factor);

	actual.discreteAxises.at(Diag::ScaleId::lightness) =
	    interpolate(source.discreteAxises.at(Diag::ScaleId::lightness),
	        target.discreteAxises.at(Diag::ScaleId::lightness),
	        factor);
}

void Morph::Color::transform(const Marker &source,
				 const Marker &target,
				 Marker &actual,
				 double factor) const
{
	actual.color = interpolate(source.color, target.color, factor);
	actual.selected = interpolate(source.selected, target.selected, factor);
}
