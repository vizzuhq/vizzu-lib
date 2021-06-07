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
	else if (Text::SmartString::startsWith(path, "filter"))
	{
		setFilter(path, value);
	}
	else
	{
		accessors.at(path)(*setter, value);
	}
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

void Descriptor::setFilter(const std::string &path,
    const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');
	if (parts.at(0) != "filter")
		throw std::logic_error("invalid parameter: " + parts.at(0));

	if (parts.size() == 2)
	{
		if (value == "push") setter->pushFilter();
		else if (value == "clear") setter->clearFilter();
		else throw std::logic_error("invalid filter command: " + value);
	}
	else
	{
		auto orIndex = std::stoi(parts.at(1));
		auto &seriesId = parts.at(2);

		setter->setFilterCondition(orIndex, seriesId, value);
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

	res.emplace("direction",
	[](OptionsSetter &setter, const std::string &value)
	{
		typedef Direction::EnumType D;
		int direction = (int)(D)Direction(value);
		setter.setAngle(direction);
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
