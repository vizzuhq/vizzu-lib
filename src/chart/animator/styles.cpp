#include "chart/animator/styles.h"

#include "base/refl/struct.h"
#include "base/math/interpolation.h"

using namespace Vizzu;
using namespace Vizzu::Anim::Morph;

template <typename T> class StyleMorph : public ::Anim::IElement
{
public:
	StyleMorph(const T &source, const T &target, T &actual) :
	    source(source),
	    target(target),
	    actual(actual)
	{}

	void transform(double factor) override
	{
		*actual = Math::interpolate(*source, *target, factor);
	}

	const T &source;
	const T &target;
	T &actual;
};

StyleMorphFactory::StyleMorphFactory(
	const Styles::Chart &source,
    const Styles::Chart &target,
    Styles::Chart &actual,
    ::Anim::Group &group) :
	group(group)
{
	pSource = reinterpret_cast<const std::byte *>(&source);
	pTarget = reinterpret_cast<const std::byte *>(&target);
	pActual = reinterpret_cast<std::byte *>(&actual);

	actual.visit(*this);
}

template <typename T>
StyleMorphFactory &StyleMorphFactory::operator()(T &value,
    const char *)
{
	if constexpr (Refl::isReflectable<T, StyleMorphFactory>)
	{
		value.visit(*this);
	}
	else if constexpr (
	   !std::is_same_v<typename T::value_type, Gfx::Font::Style>
	&& !std::is_same_v<typename T::value_type, Styles::Marker::RenderingOrder>
	&& !std::is_same_v<typename T::value_type, Text::NumberFormat>
	&& !std::is_same_v<typename T::value_type, Styles::MarkerLabel::Format>
	&& !std::is_same_v<typename T::value_type, Gfx::ColorPalette>
	)
	{
		auto offset = reinterpret_cast<std::byte *>(&value) - pActual;
		const T &source = *reinterpret_cast<const T *>(pSource + offset);
		const T &target = *reinterpret_cast<const T *>(pTarget + offset);

		if (*source != *target)
		{
			auto morph = std::make_unique<StyleMorph<T>>
				(source, target, value);

			group.addElement(std::move(morph),
				::Anim::Options(std::chrono::milliseconds(500)));
		}
	}
	return *this;
}
