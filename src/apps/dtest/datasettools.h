#ifndef DTEST_DATASET_TOOLS_H
#define DTEST_DATASET_TOOLS_H

#include "dataset/dataset.h"
#include "csvloader.h"

void datasetDump(const Vizzu::Dataset::Dataset& ds);
void datasetFromCSV(const CSVTable& table, Vizzu::Dataset::Dataset& ds);
void selectSeriesTypes(Vizzu::Dataset::Dataset& ds);

#endif //DTEST_DATASET_TOOLS_H