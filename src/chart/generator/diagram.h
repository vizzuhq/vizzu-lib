#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <array>
#include <memory>
#include <unordered_map>

#include "chart/main/style.h"
#include "chart/options/options.h"
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
	typedef std::unordered_map<uint64_t, std::map<uint64_t, uint64_t>> Buckets;
	typedef std::vector<std::pair<std::string, std::string>> CellInfo;
	typedef std::vector<Marker> Markers;

	struct MarkerInfoContent {
		Options::MarkerId markerId;
		CellInfo content;

		MarkerInfoContent();
		MarkerInfoContent(const Marker& marker, Data::DataCube *dataCube = nullptr);
		operator bool() const;
		bool operator==(const MarkerInfoContent& op) const;
	};

	typedef ::Anim::Interpolated<MarkerInfoContent> MarkerInfo;
	typedef std::map<Options::MarkerId, MarkerInfo> MarkersInfo;

	static bool dimensionMatch(const Diagram &a, const Diagram &b);

	Math::FuzzyBool anySelected;
	Math::FuzzyBool anyAxisSet;
	Axises axises;
	DiscreteAxises discreteAxises;
	Math::FuzzyBool keepAspectRatio;

	Diagram(const Diagram &other) = default;
	Diagram(DiagramOptionsPtr options, const Diagram &other);
	Diagram(const Data::DataTable &dataTable, 
		DiagramOptionsPtr opts, 
		Styles::Chart style,
		bool setAutoParams = true);
	const Markers &getMarkers() const { return markers; }
	Markers &getMarkers() { return markers; }
	const MarkersInfo &getMarkersInfo() const { return markersInfo; }
	MarkersInfo &getMarkersInfo() { return markersInfo; }
	DiagramOptionsPtr getOptions() const { return options; }
	const Data::DataCube &getDataCube() const { return dataCube; }
	const ScalesStats &getStats() const { return stats; }
	const Styles::Chart &getStyle() const { return style; }
	Styles::Chart &getStyle() { return style; }
	const Data::DataTable &getTable() const { return dataTable; };
	void detachOptions();
	bool isEmpty() const;

private:
	const Data::DataTable &dataTable;
	DiagramOptionsPtr options;
	Styles::Chart style;
	Data::DataCube dataCube;
	ScalesStats stats;
	Markers markers;
	MarkersInfo markersInfo;

	Buckets mainBuckets;
	Buckets subBuckets;

	void generateMarkers(const Data::DataCube &dataCube, const Data::DataTable &table);
	void generateMarkersInfo();
	void linkMarkers(const Buckets &buckets, bool main);
	void normalizeXY();
	void calcAxises(const Data::DataTable &dataTable);
	Axis calcAxis(ScaleId type, const Data::DataTable &dataTable);
	void calcDiscreteAxises(const Data::DataTable &table);
	void calcDiscreteAxis(ScaleId type,
	    const Data::DataTable &table);
	void addAlignment();
	void addSeparation();
	void normalizeSizes();
	void normalizeColors();
	void recalcStackedLineChart();
	std::vector<std::pair<uint64_t, double>> 
	sortedBuckets(const Buckets &buckets, bool main);
	void clearEmptyBuckets(const Buckets &buckets, bool main);
};

typedef std::shared_ptr<Diagram> DiagramPtr;

struct DiagramParent {
	virtual ~DiagramParent() {}
	virtual DiagramPtr getDiagram() const = 0;
};

}
}

#endif
