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

void AbstractMorph::transform(double factor)
{
	transform(source, target, actual, factor);

	transform(*source.getOptions(), *target.getOptions(),
		*actual.options, factor);

	for (auto i = 0u; i < source.getItems().size(); i++)
	{
		transform(source.getItems()[i], target.getItems()[i],
			actual.items[i], factor);
	}
}

void CoordinateSystem::transform(const DiagramOptions &source,
							const DiagramOptions &target,
							DiagramOptions &actual,
							double factor) const
{
	actual.polar.set(interpolate(source.polar.get(), target.polar.get(), factor));
	actual.angle.set(interpolate(source.angle.get(), target.angle.get(), factor));
}

void Enable::transform(const DiagramItem &source,
				   const DiagramItem &target,
				   DiagramItem &actual,
				   double factor) const
{
	actual.enabled = interpolate(source.enabled, target.enabled, factor);
}

void Shape::transform(const DiagramOptions &source,
				  const DiagramOptions &target,
				  DiagramOptions &actual,
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

void Horizontal::transform(const DiagramOptions &source,
				  const DiagramOptions &target,
				  DiagramOptions &actual,
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

void Horizontal::transform(const DiagramItem &source,
					   const DiagramItem &target,
					   DiagramItem &actual,
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

void Vertical::transform(const DiagramItem &source,
					 const DiagramItem &target,
					 DiagramItem &actual,
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

void Morph::Color::transform(const DiagramOptions &source,
    const DiagramOptions &target,
    DiagramOptions &actual,
    double factor) const
{
	actual.legend.set(interpolate(source.legend.get(),
	    target.legend.get(),
	    factor));
}

void Morph::Color::transform(const DiagramItem &source,
				 const DiagramItem &target,
				 DiagramItem &actual,
				 double factor) const
{
	actual.color = interpolate(source.color, target.color, factor);
	actual.selected = interpolate(source.selected, target.selected, factor);
}
