#ifndef SCALES_H
#define SCALES_H

#include <array>
#include <functional>
#include <vector>

#include "base/type/uniquetype.h"
#include "data/datacube/datacubeoptions.h"

#include "scale.h"

namespace Vizzu
{
namespace Diag
{

class Scales
{
public:
	struct Pos
	{
		ScaleId scaleId;
		int position;
	};

	Scales();

	bool anyAxisSet() const;
	bool oneAxisSet() const;
	bool bothAxisSet() const;
	bool isEmpty() const;

	Data::DataCubeOptions::IndexSet getDimensions() const;
	Data::DataCubeOptions::IndexSet getSeries() const;
	Data::DataCubeOptions::IndexSet getDimensions(
	    const std::vector<ScaleId> &scaleTypes) const;
	Data::DataCubeOptions::IndexSet getRealSeries(
	    const std::vector<ScaleId> &scaleTypes) const;
	Data::DataCubeOptions getDataCubeOptions() const;

	const Scale &at(const ScaleId &id) const;
	Scale &at(const ScaleId &id);
	ScaleId getEmptyAxisId() const;

	std::pair<bool, Scale::OptionalIndex> addSeries(const ScaleId &id,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	bool removeSeries(const Data::SeriesIndex &index);
	bool removeSeries(const ScaleId &id,
	    const Data::SeriesIndex &index);
	bool clearSeries(const ScaleId &id);

	bool isSeriesUsed(const Data::SeriesIndex &index) const;
	bool isSeriesUsed(const std::vector<ScaleId> &scaleTypes,
	    const Data::SeriesIndex &index) const;
	size_t count(const Data::SeriesIndex &index) const;
	std::list<ScaleId> find(const Data::SeriesIndex &index) const;
	std::list<Scales::Pos> findPos(
	    const Data::SeriesIndex &index) const;

	void reset();

	bool operator==(const Scales &other) const;

	void visitAll(
	    const std::function<void(ScaleId, const Scale &)> &visitor)
	    const;

	Scales shadow() const;

private:
	std::array<Scale, ScaleId::EnumInfo::count()> scales;
};

}
}

#endif
