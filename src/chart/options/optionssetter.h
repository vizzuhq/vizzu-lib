#ifndef OPTIONSSETTER_H
#define OPTIONSSETTER_H

#include "base/util/event.h"
#include "chart/main/style.h"
#include "options.h"

namespace Vizzu
{

namespace Data { class DataTable; }

namespace Diag
{

class OptionsSetter
{
public:
	typedef Util::Event<> OnFinished;
	typedef std::function<void(ScaleId, Data::SeriesIndex)>
	    OnContinousReplaced;

	OnFinished onFinished;
	OnContinousReplaced onContinousReplaced;

	OptionsSetter(Options &options,
				  const OnFinished::Listener &onFinished
				  = OnFinished::Listener());
	virtual ~OptionsSetter();

	OptionsSetter &clearSeries(const ScaleId &scaleId);

	OptionsSetter &addSeries(const ScaleId &scaleId,
	    const std::string &seriesName,
	    std::optional<size_t> pos = std::nullopt);

	OptionsSetter &deleteSeries(const ScaleId &scaleId,
	    const std::string &seriesName);

	virtual OptionsSetter &addSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	virtual OptionsSetter &deleteSeries(const ScaleId &scaleId,
	    const Data::SeriesIndex &index);
	virtual OptionsSetter &setShape(const ShapeType::Type &type);
	virtual OptionsSetter &setAlign(const Base::Align::Type &alignType);
	virtual OptionsSetter &setPolar(bool value);
	virtual OptionsSetter &setSplitted(bool value);
	virtual OptionsSetter &rotate(double ccwQuadrant);
	virtual OptionsSetter &setAngle(double ccwQuadrant);
	virtual OptionsSetter &setHorizontal(bool horizontal);
	virtual OptionsSetter &setFilter(const Data::Filter &filter);
	virtual OptionsSetter &setLabelLevel(const ScaleId &scaleId, int level);
	virtual OptionsSetter &setSorted(bool value);
	virtual OptionsSetter &setReverse(bool value);
	virtual OptionsSetter &setRangeMin(const ScaleId &scaleId,
	    const OptionalScaleExtrema &value);
	virtual OptionsSetter &setRangeMax(const ScaleId &scaleId,
	    const OptionalScaleExtrema &value);
	virtual OptionsSetter &setStackable(const ScaleId &scaleId, bool value);
	virtual OptionsSetter &setTitle(const std::optional<std::string> &title);
	virtual OptionsSetter &setLegend(const Options::Legend &legend);
	virtual OptionsSetter &setTitle(const ScaleId &scaleId,
	    const std::string &title);
	virtual OptionsSetter &setAxisLine(const ScaleId &scaleId, Base::AutoBool enable);
	virtual OptionsSetter &setAxisLabels(const ScaleId &scaleId, Base::AutoBool enable);
	virtual OptionsSetter &setTicks(const ScaleId &scaleId, Base::AutoBool enable);
	virtual OptionsSetter &setGuides(const ScaleId &scaleId, Base::AutoBool enable);
	virtual OptionsSetter &setMarkerGuides(const ScaleId &scaleId, Base::AutoBool enable);
	virtual OptionsSetter &setInterlacing(const ScaleId &scaleId, Base::AutoBool enable);
	virtual OptionsSetter &setStep(const ScaleId &scaleId, Base::AutoParam<double> step);
	virtual void replaceOptions(const Options &options);
	virtual OptionsSetter &addMarkerInfo(Options::MarkerId marker);
	virtual OptionsSetter &moveMarkerInfo(Options::MarkerId from, Options::MarkerId to);
	virtual OptionsSetter &deleteMarkerInfo(Options::MarkerId marker);
	virtual OptionsSetter &showTooltip(Options::MarkerId marker);

	bool isChanged() const { return changed; }
	const Options &getOptions() const { return options; }
	Options &getOptions() { return options; }
	void setTable(const Data::DataTable *table);
	const Data::DataTable *getTable() const { return table; }

protected:
	Options &options;
	bool changed;
	const Data::DataTable *table;
};

typedef std::shared_ptr<OptionsSetter> OptionsSetterPtr;

struct OptionsSetterParent {
	virtual ~OptionsSetterParent() {}
	virtual OptionsSetterPtr setOptions() = 0;
	virtual DiagramOptionsPtr getOptions() = 0;
	virtual const Styles::Chart &getStyle() = 0;
};

class BasicOptionsParent : public OptionsSetterParent
{
public:
	BasicOptionsParent(OptionsSetter::OnFinished::Listener onFinished)
		: onFinished(onFinished)
	{
		options = std::make_shared<Options>();
	}

	BasicOptionsParent(OptionsSetter::OnFinished::Listener onFinished,
	    const Options &options,
	    const Styles::Chart &style) :
	    onFinished(onFinished)
	{
		this->options = std::make_shared<Options>(options);
		this->style = style;
	}

	Diag::OptionsSetterPtr setOptions() override
	{
		return std::make_shared<OptionsSetter>(*options, onFinished);
	}

	DiagramOptionsPtr getOptions() override { return options; }
	const Styles::Chart &getStyle() override { return style; }

protected:
	OptionsSetter::OnFinished::Listener onFinished;
	std::shared_ptr<Options> options;
	Styles::Chart style;
};


}
}

#endif
