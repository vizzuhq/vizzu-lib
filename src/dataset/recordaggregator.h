#ifndef DATASET_RECORDAGGREGATOR_H
#define DATASET_RECORDAGGREGATOR_H

#include "aggregators.h"
#include "generators.h"
#include "interfaces.h"
#include "types.h"

namespace Vizzu
{
namespace Dataset
{

class RecordAggregator
	: public AbstractTableGenerator
{
public:
	using series_ptr = std::shared_ptr<OriginalSeries>;

	class SeriesMarker {
	public:
		std::string newSeriesName;
		std::string discreteSeriesName;
		AbstractSeriesGenerator* generator;
		AbstractSeriesAggregator* aggregator;
		int aggregatorIndex;
		series_ptr tableSeries;

		template<typename T>
		SeriesMarker(const char* name, const T& arg) :
			newSeriesName(name), generator(nullptr), aggregator(nullptr)
		{
			if constexpr (std::is_base_of<AbstractSeriesAggregator, T>())
				aggregator = new T(arg);
			else if constexpr (std::is_base_of<AbstractSeriesGenerator, T>())
				generator = new T(arg);
			else
				discreteSeriesName = arg;
		}
	};

	using table_ptr = std::shared_ptr<GeneratedTable>;
	using marker_vector = std::vector<SeriesMarker>;

    struct AggregatorItem {
        uint64_t hash;
        ConstantSeriesPtr series;
    };

	using aggregator_vector = std::vector<AggregatorItem>;

public:
	RecordAggregator(Dataset &dataset);
	
    void generate() override;
    void setOutput(const AbstractTableGenerator::output_table_ptr& output) override;
	void setFilter(const FilterPtr &);

	template <typename... Args>
	void setup(Args... args) {
		markers.reserve(64);
		internalSetup(args...);
	}

protected:
	Dataset &dataset;
	marker_vector markers;
	output_table_ptr output;

	int processMarkers(aggregator_vector& aggregators);
	RangePtr collectRecords(int count, const aggregator_vector& aggregators);
	void generateRecords(const RangePtr& range, const aggregator_vector& aggregators);

	template <typename T, typename... Args>
	void internalSetup(const T& marker, Args... args) {
		if constexpr (sizeof...(Args) != 0) {
			markers.push_back(marker);
			setup(args...);
		}
		else
			markers.shrink_to_fit();
	}
};

}
}

#endif // DATASET_RECORDAGGREGATOR_H
