#include "seriestype.h"

using namespace Vizzu::Data;

const SeriesType SeriesType::Index =
    SeriesType(false, CT::Dimension, CT::Dimension, 1u, "index");
const SeriesType SeriesType::Dimension =
    SeriesType(true, CT::Dimension, CT::Dimension, 2u, "dimension");
const SeriesType SeriesType::Exists = SeriesType(false,
    CT::Measure,
    CT::Measure,
    AT::Exists,
    "exists");
const SeriesType SeriesType::Sum =
    SeriesType(true, CT::Measure, CT::Measure, AT::Sum, "sum");
const SeriesType SeriesType::Count = SeriesType(false,
    CT::Measure,
    CT::Measure,
    AT::Count,
    "count");
const SeriesType SeriesType::Min =
    SeriesType(true, CT::Measure, CT::Measure, AT::Min, "min");
const SeriesType SeriesType::Max =
    SeriesType(true, CT::Measure, CT::Measure, AT::Max, "max");
const SeriesType SeriesType::Mean =
    SeriesType(true, CT::Measure, CT::Measure, AT::Mean, "mean");
const SeriesType SeriesType::Distinct = SeriesType(true,
    CT::Measure,
    CT::Dimension,
    AT::Distinct,
    "distinct");

const std::initializer_list<SeriesType> SeriesType::constTypes{
    SeriesType::Index,
    SeriesType::Dimension,
    SeriesType::Exists,
    SeriesType::Sum,
    SeriesType::Count,
    SeriesType::Min,
    SeriesType::Max,
    SeriesType::Mean,
    SeriesType::Distinct};

SeriesType SeriesType::fromString(std::string_view name, bool throws)
{
	for (auto type : constTypes)
		if (name == type.name) return type;

	if (throws)
		throw std::logic_error(
		    "not recognized series type: " + std::string(name));

	else
		return SeriesType();
}
