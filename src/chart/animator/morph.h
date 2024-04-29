#ifndef MORPH_H
#define MORPH_H

#include <memory>

#include "base/anim/element.h"

#include "options.h"

namespace Vizzu::Gen
{
class Plot;
class Options;
class Marker;
}

namespace Vizzu::Anim::Morph
{

class AbstractMorph : public ::Anim::IElement
{
protected:
	using Dia = Gen::Plot;
	using Opt = Gen::Options;
	using Marker = Gen::Marker;

public:
	AbstractMorph(const Dia &source, const Dia &target, Dia &actual);
	~AbstractMorph() override = default;

	static std::unique_ptr<AbstractMorph> create(SectionId sectionId,
	    const Dia &source,
	    const Dia &target,
	    Dia &actual);
	void transform(double factor) override;
	virtual void
	transform(const Dia &, const Dia &, Dia &, double) const
	{}
	virtual void
	transform(const Opt &, const Opt &, Opt &, double) const
	{}
	virtual void
	transform(const Marker &, const Marker &, Marker &, double) const
	{}

protected:
	const Dia &source;
	const Dia &target;
	Dia &actual;
};

class CoordinateSystem : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void
	transform(const Opt &, const Opt &, Opt &, double) const override;
};

class Show : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Hide : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Shape : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void
	transform(const Opt &, const Opt &, Opt &, double) const override;
};

class Horizontal : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void
	transform(const Dia &, const Dia &, Dia &, double) const override;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Connection : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void
	transform(const Opt &, const Opt &, Opt &, double) const override;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Vertical : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void
	transform(const Dia &, const Dia &, Dia &, double) const override;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Color : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	void
	transform(const Dia &, const Dia &, Dia &, double) const override;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

}

#endif
