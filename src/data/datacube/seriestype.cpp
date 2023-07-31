#include "seriestype.h"

using namespace Vizzu::Data;

const SeriesType SeriesType::Index =
    SeriesType(false, CT::dimension, CT::dimension, 1U, "index");
const SeriesType SeriesType::Dimension =
    SeriesType(true, CT::dimension, CT::dimension, 2U, "dimension");
const SeriesType SeriesType::Exists = SeriesType(false,
    CT::measure,
    CT::measure,
    AT::Exists,
    "exists");
const SeriesType SeriesType::Sum =
    SeriesType(true, CT::measure, CT::measure, AT::Sum, "sum");
const SeriesType SeriesType::Count = SeriesType(false,
    CT::measure,
    CT::measure,
    AT::Count,
    "count");
const SeriesType SeriesType::Min =
    SeriesType(true, CT::measure, CT::measure, AT::Min, "min");
const SeriesType SeriesType::Max =
    SeriesType(true, CT::measure, CT::measure, AT::Max, "max");
const SeriesType SeriesType::Mean =
    SeriesType(true, CT::measure, CT::measure, AT::Mean, "mean");
const SeriesType SeriesType::Distinct = SeriesType(true,
    CT::measure,
    CT::dimension,
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

	return {};
}
