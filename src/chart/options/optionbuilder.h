#ifndef DIAG_OPTIONBUILDER_H
#define DIAG_OPTIONBUILDER_H

#include "optionssetter.h"

namespace Vizzu
{

namespace Data { class DataTable; }

namespace Diag
{

struct Polar { bool value; };
struct Horizontal { bool value; };

class OptionsBuilder
{
public:
	OptionsBuilder(DiagramOptions &options);

	OptionsBuilder &operator<<(const Data::DataTable &table);
	OptionsBuilder &operator<<(const ShapeType::Type &t);
	OptionsBuilder &operator<<(const Scale::Type &type);
	OptionsBuilder &operator<<(const Scales::Index &index);
	OptionsBuilder &operator<<(const Base::Align::Type &alignType);
	OptionsBuilder &operator<<(const Data::SeriesIndex &index);
	OptionsBuilder &operator<<(const char *seriesName);
	OptionsBuilder &operator<<(const Polar &polar);
	OptionsBuilder &operator<<(const Horizontal &horizontal);

private:
	OptionsSetter setter;
	Scales::Id scaleId;
};

template <typename T>
OptionsBuilder operator<<(DiagramOptions &options, const T &v) {
	return OptionsBuilder(options) << std::ref(v);
}

}
}

#endif
