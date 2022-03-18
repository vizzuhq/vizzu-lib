#include "seriestype.h"

using namespace Vizzu::Data;

const SeriesType SeriesType::Index		= SeriesType( false, CT::Discrete, 1u, "index" );
const SeriesType SeriesType::Discrete	= SeriesType( true,  CT::Discrete, 2u, "discrete" );
const SeriesType SeriesType::Sum		= SeriesType( true,  CT::Continous, AT::Sum, "sum");
const SeriesType SeriesType::Count		= SeriesType( false, CT::Continous, AT::Count, "count" );
const SeriesType SeriesType::Min		= SeriesType( true,  CT::Continous, AT::Min, "min" );
const SeriesType SeriesType::Max		= SeriesType( true,  CT::Continous, AT::Max, "max" );
const SeriesType SeriesType::Mean		= SeriesType( true,  CT::Continous, AT::Mean, "mean" );
const SeriesType SeriesType::Distinct	= SeriesType( false, CT::Continous, AT::Distinct, "distinct" );

const std::vector<SeriesType> SeriesType::constTypes
{
	SeriesType::Index,
	SeriesType::Discrete,
	SeriesType::Sum,
	SeriesType::Count,
	SeriesType::Min,
	SeriesType::Max,
	SeriesType::Mean,
	SeriesType::Distinct
};

SeriesType SeriesType::fromString(const std::string &name)
{
	for (auto type : constTypes)
		if (name == type.name) return type;
	throw std::logic_error("not recognized series type: " + name);
}

void SeriesType::deduceName()
{
	for (auto type : constTypes)
		if (*this == type) name = type.name;
}
