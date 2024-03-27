#ifndef VIZZU_DATAFRAME_AGGREGATORS_H
#define VIZZU_DATAFRAME_AGGREGATORS_H

#include "../interface.h"
#include "base/refl/auto_enum.h"

namespace Vizzu::dataframe
{
extern const Refl::EnumArray<aggregator_type, custom_aggregator>
    aggregators;
}

#endif // VIZZU_DATAFRAME_AGGREGATORS_H
