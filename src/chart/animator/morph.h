#ifndef MORPH_H
#define MORPH_H

#include <memory>

#include "base/anim/element.h"
#include "base/math/interpolation.h"
#include "base/math/ratio.h"
#include "chart/generator/plot.h"
#include "chart/options/options.h"

#include "options.h"

namespace Vizzu
{
namespace Anim
{
namespace Morph
{

class AbstractMorph : public ::Anim::IElement
{
protected:
	typedef Gen::Plot Dia;
	typedef Gen::Options Opt;
	typedef Gen::Marker Marker;

public:
	AbstractMorph(const Dia &source, const Dia &target, Dia &actual);
	virtual ~AbstractMorph() {}

	static std::unique_ptr<AbstractMorph> create(SectionId sectionId,
	    const Dia &source,
	    const Dia &target,
	    Dia &actual);
	void transform(double factor) override;
	virtual std::string name() const = 0;
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
	std::string name() const override { return "CoordSys"; }
	void
	transform(const Opt &, const Opt &, Opt &, double) const override;
};

class Show : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	std::string name() const override { return "Show"; }
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Hide : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	std::string name() const override { return "Hide"; }
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

class Shape : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	std::string name() const override { return "Shape"; }
	void
	transform(const Opt &, const Opt &, Opt &, double) const override;
};

class Horizontal : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	std::string name() const override { return "Horizontal"; }
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
	std::string name() const override { return "Connection"; }
	void transform(const Opt&, const Opt&, Opt&, double) const override;
	void transform(const Marker&, const Marker&, Marker&, double) const override;
};

class Vertical : public AbstractMorph
{
public:
	using AbstractMorph::AbstractMorph;
	std::string name() const override { return "Vertical"; }
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
	std::string name() const override { return "Color"; }
	void
	transform(const Dia &, const Dia &, Dia &, double) const override;
	void transform(const Marker &,
	    const Marker &,
	    Marker &,
	    double) const override;
};

}
}
}

#endif
