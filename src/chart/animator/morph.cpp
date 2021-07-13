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
	case SectionId::EnumType::enable:
		return std::make_unique<Enable>(source, target, actual);
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

void Enable::transform(const Marker &source,
				   const Marker &target,
				   Marker &actual,
				   double factor) const
{
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
	actual.axises.at(Diag::Scale::Type::X) =
	    interpolate(source.axises.at(Diag::Scale::Type::X),
	        target.axises.at(Diag::Scale::Type::X),
	        factor);

	actual.discreteAxises.at(Diag::Scale::Type::X) =
	    interpolate(source.discreteAxises.at(Diag::Scale::Type::X),
	        target.discreteAxises.at(Diag::Scale::Type::X),
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
	actual.axises.at(Diag::Scale::Type::Y) =
	    interpolate(source.axises.at(Diag::Scale::Type::Y),
	        target.axises.at(Diag::Scale::Type::Y),
	        factor);

	actual.discreteAxises.at(Diag::Scale::Type::Y) =
	    interpolate(source.discreteAxises.at(Diag::Scale::Type::Y),
	        target.discreteAxises.at(Diag::Scale::Type::Y),
	        factor);

	actual.axises.at(Diag::Scale::Type::Size) =
	    interpolate(source.axises.at(Diag::Scale::Type::Size),
	        target.axises.at(Diag::Scale::Type::Size),
	        factor);

	actual.discreteAxises.at(Diag::Scale::Type::Size) =
	    interpolate(source.discreteAxises.at(Diag::Scale::Type::Size),
	        target.discreteAxises.at(Diag::Scale::Type::Size),
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

	actual.axises.at(Diag::Scale::Type::Color) =
	    interpolate(source.axises.at(Diag::Scale::Type::Color),
	        target.axises.at(Diag::Scale::Type::Color),
	        factor);

	actual.discreteAxises.at(Diag::Scale::Type::Color) =
	    interpolate(source.discreteAxises.at(Diag::Scale::Type::Color),
	        target.discreteAxises.at(Diag::Scale::Type::Color),
	        factor);

	actual.axises.at(Diag::Scale::Type::Lightness) =
	    interpolate(source.axises.at(Diag::Scale::Type::Lightness),
	        target.axises.at(Diag::Scale::Type::Lightness),
	        factor);

	actual.discreteAxises.at(Diag::Scale::Type::Lightness) =
	    interpolate(source.discreteAxises.at(Diag::Scale::Type::Lightness),
	        target.discreteAxises.at(Diag::Scale::Type::Lightness),
	        factor);
}

void Morph::Color::transform(const Diag::Options &source,
    const Diag::Options &target,
    Diag::Options &actual,
    double factor) const
{
	actual.legend.set(interpolate(source.legend.get(),
	    target.legend.get(),
	    factor));
}

void Morph::Color::transform(const Marker &source,
				 const Marker &target,
				 Marker &actual,
				 double factor) const
{
	actual.color = interpolate(source.color, target.color, factor);
	actual.selected = interpolate(source.selected, target.selected, factor);
}
