#ifndef DATASET_RECORDAGGREGATOR_H
#define DATASET_RECORDAGGREGATOR_H

#include "types.h"
#include "interfaces.h"
#include "generators.h"
#include "aggregators.h"

namespace Vizzu {
namespace Dataset {

class RecordAggregator : public AbstractTableGenerator {
public:
    class SeriesMarker {
    public:
        RecordAggregator* owner;
        std::string seriesName;
    };

    class GeneratedSeries : public SeriesMarker {
    public:
        GeneratedSeries(const char* nameInTable, const AbstractSeriesGenerator& inst);
    };

    class DiscreteSeries : public SeriesMarker {
    public:
        DiscreteSeries(const char* nameInTable, const char* nameInDataset);
    };

    class AggregatedSeries : public SeriesMarker {
    public:
        AggregatedSeries(const char* nameInTable, const AbstractSeriesAggregator& inst);
    };

public:
    RecordAggregator(Dataset& dataset);

    template<class ...Args>
    void setup(Args... args) {
        setup(args...);
    }

    template<class ...Args>
    void setup(SeriesMarker& gi, Args... args) {
        setup(gi, args...);
    }

    void setup() {
    }

protected:
    Dataset& dataset;
};

}
}

#endif // DATASET_RECORDAGGREGATOR_H
