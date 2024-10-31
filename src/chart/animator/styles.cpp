#include "styles.h"

#include <memory>

#include "base/anim/group.h"
#include "base/anim/options.h"
#include "base/refl/auto_struct.h"
#include "base/text/numberscale.h"
#include "base/text/smartstring.h"
#include "chart/main/style.h"

namespace Vizzu::Anim::Morph
{

StyleMorphFactory::StyleMorphFactory(const Styles::Chart &source,
    const Styles::Chart &target,
    Styles::Chart &actual) :
    pActual(std::addressof(actual)),
    pSource(std::addressof(source)),
    pTarget(std::addressof(target))
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
    requires(requires(StyleMorph<T> &m) { m.transform(0.0); })
void StyleMorphFactory::operator()(const T &source,
    const T &target,
    T &value) const
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

template <typename T, typename PT>
    requires(std::is_same_v<PT, Text::NumberFormat>
             || std::is_same_v<PT, Text::NumberScale>
             || std::is_same_v<PT, Styles::MarkerLabel::Format>)
void StyleMorphFactory::operator()(const T &, const T &, T &) const
{}

}