#include "seriestype.h"

using namespace Vizzu::Data;

const SeriesType SeriesType::Index =
    SeriesType(false, CT::Discrete, CT::Discrete, 1u, "index");
const SeriesType SeriesType::Discrete =
    SeriesType(true, CT::Discrete, CT::Discrete, 2u, "discrete");
const SeriesType SeriesType::Exists = SeriesType(false,
    CT::Continous,
    CT::Continous,
    AT::Exists,
    "exists");
const SeriesType SeriesType::Sum =
    SeriesType(true, CT::Continous, CT::Continous, AT::Sum, "sum");
const SeriesType SeriesType::Count = SeriesType(false,
    CT::Continous,
    CT::Continous,
    AT::Count,
    "count");
const SeriesType SeriesType::Min =
    SeriesType(true, CT::Continous, CT::Continous, AT::Min, "min");
const SeriesType SeriesType::Max =
    SeriesType(true, CT::Continous, CT::Continous, AT::Max, "max");
const SeriesType SeriesType::Mean =
    SeriesType(true, CT::Continous, CT::Continous, AT::Mean, "mean");
const SeriesType SeriesType::Distinct = SeriesType(true,
    CT::Continous,
    CT::Discrete,
    AT::Distinct,
    "distinct");

const std::initializer_list<SeriesType> SeriesType::constTypes{
    SeriesType::Index,
    SeriesType::Discrete,
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
