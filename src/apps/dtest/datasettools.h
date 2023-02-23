#ifndef DTEST_DATASET_TOOLS_H
#define DTEST_DATASET_TOOLS_H

#include "dataset/dataset.h"
#include "csvloader.h"

void datasetDump(const Vizzu::DataSet::DataSet& ds);
void datasetFromCSV(const CSVTable& table, Vizzu::DataSet::DataSet& ds);
void normalizeSeriesValues(Vizzu::DataSet::DataSet& ds);

#endif //DTEST_DATASET_TOOLS_H