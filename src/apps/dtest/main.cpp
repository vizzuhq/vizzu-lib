#include <iostream>

#include "base/io/log.h"
#include "dataset/dataset.h"
#include "csvloader.h"
#include "datasettools.h"

using namespace Vizzu::DataSet;

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
    DataSet dataset;
    datasetFromCSV(csv, dataset);
    datasetDump(dataset);
    return 0;
}
