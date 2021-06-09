#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <array>
#include <memory>
#include <unordered_map>

#include "chart/main/style.h"
#include "chart/options/diagramoptions.h"
#include "data/table/datatable.h"

#include "axis.h"
#include "marker.h"
#include "scalestats.h"

namespace Vizzu
{

namespace Anim {
	class Animator;
	namespace Morph { class AbstractMorph; }
}

namespace Diag
{

class Selector;

class Diagram
{
	friend class Anim::Animator;
	friend class Anim::Morph::AbstractMorph;
	friend class Selector;
public:
	typedef std::vector<Marker> Markers;

	static bool dimensionMatch(const Diagram &a, const Diagram &b);

	Math::FuzzyBool anySelected;
	Math::FuzzyBool anyAxisSet;
	Axises axises;
	DiscreteAxises discreteAxises;
	Math::FuzzyBool keepAspectRatio;

	Diagram(const Diagram &other) = default;
	Diagram(DiagramOptionsPtr options, const Diagram &other);
	Diagram(const Data::DataTable &dataTable, DiagramOptionsPtr opts, Styles::Chart style);
	const Markers &getMarkers() const { return markers; }
	DiagramOptionsPtr getOptions() const { return options; }
	const Data::DataCube &getDataCube() const { return dataCube; }
	const ScalesStats &getStats() const { return stats; }
	const Styles::Chart &getStyle() const { return style; }
	Styles::Chart &getStyle() { return style; }
	void detachOptions();
	bool isEmpty() const;

private:
	DiagramOptionsPtr options;
	Styles::Chart style;
	Data::DataCube dataCube;
	ScalesStats stats;
	Markers markers;

	typedef std::unordered_map<
		uint64_t,
		std::map<uint64_t, uint64_t>
	> Buckets;

	Buckets mainBuckets;
	Buckets subBuckets;

	void generateMarkers(const Data::DataCube &dataCube, const Data::DataTable &table);
	void linkMarkers(const Buckets &buckets, bool main);
	bool addLayoutIfNeeded();
	void normalizeXY();
	void calcAxises(const Data::DataTable &dataTable);
	Axis calcAxis(Scale::Type type, const Data::DataTable &dataTable);
	void calcDiscreteAxises(const Data::DataTable &table);
	void calcDiscreteAxis(Scale::Type type,
	    const Data::DataTable &table);
	void addAlignment();
	void addSeparation();
	void normalizeSizes();
	void normalizeColors();
	void recalcStackedLineChart();
};

typedef std::shared_ptr<Diagram> DiagramPtr;

struct DiagramParent {
	virtual ~DiagramParent() {}
	virtual DiagramPtr getDiagram() const = 0;
};

}
}

#endif
