#include "descriptor.h"

#include "base/io/log.h"
#include "base/conv/parse.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

const Descriptor::Accessors Descriptor::accessors = Descriptor::initAccessors();

void Descriptor::setParam(
	const std::string &path,
    const std::string &value)
{
	if (Text::SmartString::startsWith(path, "channels."))
	{
		setChannelParam(path, value);
	}
	else
	{
		accessors.at(path)(*setter, value);
	}
}

void Descriptor::setFilter(Filter filter, ReleaseFilter deleter)
{
	struct Releaser {
		Releaser(Filter filter, ReleaseFilter deleter) : 
			filter(filter),
			deleter(deleter) 
		{}
		~Releaser() {
			deleter(filter);
		}
		Filter filter;
		ReleaseFilter deleter;
	};

	Data::Filter::Function func;

	if (filter)
	{
		std::shared_ptr<Releaser> releaser;
		if (deleter) {
			releaser = std::make_shared<Releaser>(filter, deleter);
		}

		func = [&, filter, releaser](const Data::RowWrapper &row)
		{
			return filter(static_cast<const void *>(&row));
		};
	}
	setter->setFilter(func);
}

void Descriptor::setChannelParam(
	const std::string &path,
	const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');
	auto id = Scales::Id(Diag::toScaleType(parts.at(1)));
	auto property = parts.at(2);

	if (property == "title") {
		setter->setTitle(id, value);
	}
	else if (property == "attach")
	{
		setter->addSeries(id, value);
	}
	else if (property == "detach")
	{
		setter->deleteSeries(id, value);
	}
	else if (property == "clear")
	{
		setter->clearSeries(id);
	}
	else if (property == "stackable")
	{
		setter->setStackable(id, Conv::parse<bool>(value));
	}
	else if (property == "range")
	{
		auto values = Text::SmartString::split(value, ',');

		// todo: range should be set using { min: "1%", max: "50" }
		// todo: can interpoate between different range units?
		auto unit = (values.size() >= 3 && values[2] == "%")
			? Type::SimpleUnit::relative : Type::SimpleUnit::absolute;

		Math::Range<double> range(
			std::stod(values.at(0)),
		    std::stod(values.at(1)));

		Type::PhysicalValue<Math::Range<double>> value(range, unit);

		setter->setRange(id, value);
	}
	else if (property == "labelLevel")
	{
		setter->setLabelLevel(id, Conv::parse<uint64_t>(value));
	}
}

Descriptor::Accessors Descriptor::initAccessors()
{
	Accessors res;

	res.emplace("title",
	[](OptionsSetter &setter, const std::string &value)
	{
		setter.setTitle(
		    Conv::parse<std::optional<std::string>>(value));
	});

	res.emplace("legend",
	[](OptionsSetter &setter, const std::string &value)
	{
		//todo: use refl::enum for Scale::Type
		if (value == "null") setter.setLegend(std::nullopt);
		else setter.setLegend(toScaleType(value));
	});

    res.emplace("coordSystem",
	[](OptionsSetter &setter, const std::string &value)
	{
		typedef CoordSystem::EnumType CS;
		CS coordSys = CoordSystem(value);
		setter.setPolar(coordSys == CS::polar);
	});

	res.emplace("rotate",
	[](OptionsSetter &setter, const std::string &value)
	{
		setter.rotate(Conv::parse<double>(value) / 90);
	});

	res.emplace("geometry",
	[](OptionsSetter &setter, const std::string &value)
	{
		typedef Geometry::EnumType G;
		G geometry = Geometry(value);
		setter.setShape((ShapeType::Type)(int)(geometry));
	});

	res.emplace("orientation",
	[](OptionsSetter &setter, const std::string &value)
	{
		typedef Orientation::EnumType O;
		O orientation = Orientation(value);
		setter.setHorizontal(orientation == O::horizontal);
	});

	res.emplace("bubbleChartAlgorithm",
	[](OptionsSetter &setter, const std::string &value)
	{
		setter.setBubbleChartAlgorithm(
			BubbleChartAlgorithm(value));
	});

	res.emplace("sort",
	[](OptionsSetter &setter, const std::string &value)
	{
		typedef Sort::EnumType S;
		S sort = Sort(value);
		setter.setSorted(sort == S::experimental);
	});

	res.emplace("reverse",
	[](OptionsSetter &setter, const std::string &value)
	{
		setter.setReverse(Conv::parse<bool>(value));
	});

	res.emplace("align",
	[](OptionsSetter &setter, const std::string &value)
	{
		typedef Align::EnumType A;
		A align = Align(value);
		auto alignType = (Base::Align::Type)(int)align;
		setter.setAlign(alignType);
	});

	res.emplace("split",
	[](OptionsSetter &setter, const std::string &value)
	{
		setter.setSplitted(Conv::parse<bool>(value));
	});

	return res;
}
