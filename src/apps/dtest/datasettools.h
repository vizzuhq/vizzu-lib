#ifndef DTEST_DATASET_TOOLS_H
#define DTEST_DATASET_TOOLS_H

#include "dataset/dataset.h"
#include "csvloader.h"

void datasetDump(const Vizzu::Dataset::Dataset& ds);
void tableDump(const Vizzu::Dataset::TablePtr& table);
void seriesRangeDump(const Vizzu::Dataset::RangePtr& range);
void datasetFromCSV(const char* fileName, Vizzu::Dataset::Dataset& ds);
void unifySeriesTypes(Vizzu::Dataset::Dataset& ds);

#endif //DTEST_DATASET_TOOLS_H