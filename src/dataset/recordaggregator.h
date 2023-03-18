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
		std::string resultName;
		std::string aggregatorName;
		AbstractSeriesGenerator* generator;
		AbstractSeriesAggregator* aggregator;
		series_ptr resultSeries;
		uint64_t aggregatorHash;
		ConstantSeriesPtr aggregatorSeries;

		template<typename T>
		SeriesMarker(const char* name, const T& arg) :
			resultName(name), generator(nullptr), aggregator(nullptr)
		{
			if constexpr (std::is_base_of<AbstractSeriesAggregator, T>())
				aggregator = new T(arg);
			else if constexpr (std::is_base_of<AbstractSeriesGenerator, T>())
				generator = new T(arg);
			else
				aggregatorName = arg;
		}
	};

	using table_ptr = std::shared_ptr<GeneratedTable>;
	using marker_vector = std::vector<SeriesMarker>;
	using index_vector = std::vector<int>;

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
	output_table_ptr table;

	int processMarkers(index_vector& input, index_vector& output);
	RangePtr collectRecords(int count, const index_vector& input);
	void generateRecords(const RangePtr& range, const index_vector& input, const index_vector& output);

	template <typename T, typename... Args>
	void internalSetup(const T& marker, Args... args) {
		markers.push_back(marker);
		if constexpr (sizeof...(Args) != 0)
			setup(args...);
		else
			markers.shrink_to_fit();
	}
};

}
}

#endif // DATASET_RECORDAGGREGATOR_H
