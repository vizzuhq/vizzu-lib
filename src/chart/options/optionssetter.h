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
	typedef std::function<void(Scales::Id, Data::SeriesIndex)>
	    OnContinousReplaced;

	OnFinished onFinished;
	OnContinousReplaced onContinousReplaced;

	OptionsSetter(Options &options,
				  const OnFinished::Listener &onFinished
				  = OnFinished::Listener());
	virtual ~OptionsSetter();

	OptionsSetter &clearSeries(const Scales::Id &scaleId);

	OptionsSetter &addSeries(const Scales::Id &scaleId,
	    const std::string &seriesName,
	    std::optional<size_t> pos = std::nullopt);

	OptionsSetter &deleteSeries(const Scales::Id &scaleId,
	    const std::string &seriesName);

	virtual OptionsSetter &addSeries(const Scales::Id &scaleId,
	    const Data::SeriesIndex &index,
	    std::optional<size_t> pos = std::nullopt);
	virtual OptionsSetter &deleteSeries(const Scales::Id &scaleId,
	    const Data::SeriesIndex &index);
	virtual OptionsSetter &setShape(const ShapeType::Type &type);
	virtual OptionsSetter &setAlign(const Base::Align::Type &alignType);
	virtual OptionsSetter &setPolar(bool value);
	virtual OptionsSetter &setSplitted(bool value);
	virtual OptionsSetter &rotate(double ccwQuadrant);
	virtual OptionsSetter &setAngle(double ccwQuadrant);
	virtual OptionsSetter &setHorizontal(bool horizontal);
	virtual OptionsSetter &setFilter(const Data::Filter &filter);
	virtual OptionsSetter &setLabelLevel(const Scales::Id &scaleId, int level);
	virtual OptionsSetter &setSorted(bool value);
	virtual OptionsSetter &setReverse(bool value);
	virtual OptionsSetter &setRange(const Scales::Id &scaleId,
	    Type::PhysicalValue<Math::Range<double>> range);
	virtual OptionsSetter &setStackable(const Scales::Id &scaleId, bool value);
	virtual OptionsSetter &setTitle(const std::optional<std::string> &title);
	virtual OptionsSetter &setLegend(std::optional<Scale::Type> type);
	virtual OptionsSetter &setTitle(const Scales::Id &scaleId,
	    const std::string &title);
	virtual OptionsSetter &setBubbleChartAlgorithm(BubbleChartAlgorithm value);
	virtual void replaceOptions(const Options &options);
	virtual OptionsSetter &addMarkerInfo(const Options::MarkerIndex& marker);
	virtual OptionsSetter &moveMarkerInfo(const Options::MarkerIndex& from,
		const Options::MarkerIndex& to);
	virtual OptionsSetter &deleteMarkerInfo(const Options::MarkerIndex& marker);
	virtual OptionsSetter &showTooltip(uint64_t markerId);

	bool isChanged() const { return changed; }
	const Options &getOptions() const { return options; }
	Options &getOptions() { return options; }
	void setTable(const Data::DataTable *table);

protected:
	Options &options;
	bool changed;
	uint64_t currentTooltipId;
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
