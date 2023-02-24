#include <iostream>

#include "base/io/log.h"
#include "dataset/dataset.h"
#include "csvloader.h"
#include "datasettools.h"

using namespace Vizzu::Dataset;

int main(int argc, char *argv[]) {
    std::cout << "Dataset test suit v0.1" << std::endl;
    if (argc != 2) {
        std::cout << "usage: dtest <csv file name>" << std::endl;
        return 0;
    }
    CSVTable csv;
    if (csv.load(argv[1]) <= 0) {
        std::cout << "Failed to open file or file is empty." << std::endl;
        return -1;
    }
    Dataset dataset1;
    datasetFromCSV(csv, dataset1);
    datasetDump(dataset1);
    selectSeriesTypes(dataset1);
    Dataset dataset2(dataset1);
    dataset1.clear();
    datasetDump(dataset2);
    return 0;
}
