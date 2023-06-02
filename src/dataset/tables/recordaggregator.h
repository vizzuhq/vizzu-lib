#ifndef DATASET_RECORDAGGREGATOR_H
#define DATASET_RECORDAGGREGATOR_H

#include "../interface.h"
#include "aggregators.h"
#include "generators.h"
#include "generatedtable.h"

namespace Vizzu {
namespace Dataset {

class RecordAggregator
	: public AbstractTableGenerator
{
public:
	using series_ptr = std::shared_ptr<RawSeries>;

	class SeriesMarker {
	public:
		std::string resultName;
		std::string aggregatorName;
		series_ptr resultSeries;
		ConstantSeriesPtr aggregatorSeries;
		std::unique_ptr<AbstractSeriesGenerator> generator;
		std::unique_ptr<AbstractSeriesAggregator> aggregator;

		template<typename T>
		SeriesMarker(const char* name, const T& arg) :
			resultName(name)
		{
			if constexpr (std::is_base_of<AbstractSeriesAggregator, T>())
				aggregator = std::make_unique<T>(arg);
			else if constexpr (std::is_base_of<AbstractSeriesGenerator, T>())
				generator = std::make_unique<T>(arg);
			else
				aggregatorName = arg;
		}
	};
	
	class GeneratedSeries : public SeriesMarker {
	public:
		template<typename T>
		GeneratedSeries(const char* n, const T& g) : SeriesMarker(n, g) {}
		GeneratedSeries(GeneratedSeries&& m) : SeriesMarker(std::move(m)) {}
	};

	class DiscreteSeries : public SeriesMarker {
	public:
		DiscreteSeries(const char* n1, const char* n2) : SeriesMarker(n1, n2) {}
		DiscreteSeries(GeneratedSeries&& m) : SeriesMarker(std::move(m)) {}
	};

	class AggregatedSeries : public SeriesMarker {
	public:
		template<typename T>
		AggregatedSeries(const char* n, const T& a) : SeriesMarker(n, a) {}
		AggregatedSeries(GeneratedSeries&& m) : SeriesMarker(std::move(m)) {}
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
		internalSetup(std::move(args)...);
	}

protected:
	Dataset &dataset;
	marker_vector markers;
	output_table_ptr table;
	FilterPtr filter;
	LinkedSeries::indices_ptr filteredIndeces;

	int processMarkers(index_vector& input, index_vector& output);
	int filterRecords(int recordCount, const index_vector& input);
	RangePtr collectRecords(int count, const index_vector& input);
	void generateRecords(const RangePtr& range, const index_vector& input, const index_vector& output);

	template <typename T, typename... Args>
	void internalSetup(T&& marker, Args... args) {
		markers.push_back(std::move(marker));
		if constexpr (sizeof...(Args) != 0)
			setup(std::move(args)...);
		else
			markers.shrink_to_fit();
	}
};

}
}

#endif // DATASET_RECORDAGGREGATOR_H
