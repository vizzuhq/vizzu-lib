#ifndef SERIESINDEX_H
#define SERIESINDEX_H

#include <base/type/uniquelist.h>
#include <dataframe/old/datatable.h>

namespace Vizzu::Data
{

using SeriesIndex = series_index_t;

using SeriesList = Type::UniqueList<SeriesIndex>;

}

#endif
