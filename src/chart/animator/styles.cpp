#include "chart/animator/styles.h"

#include "base/refl/auto_struct.h"

using namespace Vizzu;
using namespace Vizzu::Anim::Morph;
using namespace Math;
StyleMorphFactory::StyleMorphFactory(const Styles::Chart &source,
    const Styles::Chart &target,
    Styles::Chart &actual) :
    needed{},
    pActual(std::addressof(actual)),
    pSource(std::addressof(source)),
    pTarget(std::addressof(target)),
    group(nullptr),
    options(nullptr)
{}

void StyleMorphFactory::visit() const
{
	Refl::visit(*this, *pSource, *pTarget, *pActual);
}

bool StyleMorphFactory::isNeeded() const
{
	needed = false;
	visit();
	return needed;
}

void StyleMorphFactory::populate(::Anim::Group &group,
    const ::Anim::Options &options)
{
	this->group = &group;
	this->options = &options;
	visit();
	this->group = nullptr;
	this->options = nullptr;
}

template <typename T>
auto StyleMorphFactory::operator()(const T &source,
    const T &target,
    T &value) const
    -> std::void_t<decltype(std::declval<StyleMorph<T> &>().transform(
        0.0))>
{
	if (*source != *target) {
		if (group)
			group->addElement(std::make_unique<StyleMorph<T>>(source,
			                      target,
			                      value),
			    *options);
		else
			needed = true;
	}
}

template <typename T>
auto StyleMorphFactory::operator()(const T &, const T &, T &) const
    -> std::enable_if_t<
        std::is_same_v<typename T::value_type, Text::NumberFormat>
        || std::is_same_v<typename T::value_type, Text::NumberScale>
        || std::is_same_v<typename T::value_type,
            Styles::MarkerLabel::Format>
        || std::is_same_v<typename T::value_type, Gfx::ColorPalette>>
{}
