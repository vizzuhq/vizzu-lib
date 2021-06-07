#ifndef SCALES_H
#define SCALES_H

#include <vector>
#include <array>
#include <functional>

#include "scale.h"

#include "base/type/uniquetype.h"
#include "data/datacube/datacubeoptions.h"

namespace Vizzu
{
namespace Diag
{

class Scales
{
public:

	struct IndexTypeId;
	typedef Type::UniqueType<uint64_t, IndexTypeId> Index;

	typedef std::vector<Scale> ScaleList;
	typedef std::array<const Scale *, Scale::id_size> Level;

	struct Id {
		Scale::Type type;
		Index index;
		Id(Scale::Type type, Index index = Index{0})
			: type(type), index(index)
		{}
		std::string toString() const;
	};

	struct Pos
	{
		Scales::Id scaleId;
		int position;
	};

	Scales();

	Level getScales(Index index) const;

	bool anyAxisSet() const;
	bool oneAxisSet() const;
	bool bothAxisSet() const;
	bool isEmpty() const;

	bool anyScaleOfType(Scale::Type type) const;
	Scales::Index maxScaleSize() const;

	Data::DataCubeOptions::IndexSet getDimensions() const;
	Data::DataCubeOptions::IndexSet getSeries() const;
	Data::DataCubeOptions::IndexSet getDimensions(const std::vector<Scale::Type> &scaleTypes) const;
	Data::DataCubeOptions::IndexSet getRealSeries(const std::vector<Scale::Type> &scaleTypes) const;
	Data::DataCubeOptions getDataCubeOptions() const;

	const Scale &at(const Id &id, bool top = true) const;
	Scale &at(const Id &id, bool top = true);
	const Scale &at(const Scale::Type &type,
					const Index &index = Index{0},
					bool top = true) const;
	Scale &at(const Scale::Type &type,
					const Index &index = Index{0},
					bool top = true);
	Id getEmptyAxisId() const;

	const ScaleList &operator[](Scale::Type type) const;

	std::pair<bool, Scale::OptionalContinousIndex>
	addSeries(const Id &id,
			  const Data::SeriesIndex &index,
			  std::optional<size_t> pos = std::nullopt);
	bool removeSeries(const Id &id, const Data::SeriesIndex &index);
	bool clearSeries(const Id &id);

	bool isSeriesUsed(const Data::SeriesIndex &index) const;
	bool isSeriesUsed(const std::vector<Scale::Type> &scaleTypes,
					  const Data::SeriesIndex &index) const;
	size_t count(const Data::SeriesIndex &index) const;
	std::list<Scales::Id> find(const Data::SeriesIndex &index) const;
	std::list<Scales::Pos> findPos(const Data::SeriesIndex &index) const;

	void reset();

	bool operator==(const Scales& other) const;

	void visitAll(const std::function<void(Id, const Scale&)> &visitor) const;

private:
	std::array<ScaleList, Scale::Type::id_size> scales;
	std::array<Scale, Scale::Type::id_size> makeNulls() const;
	Scale &getScale(const Id &id);
};

}
}

#endif
