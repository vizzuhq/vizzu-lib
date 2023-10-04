#ifndef ANIM_GROUP
#define ANIM_GROUP

#include <memory>
#include <vector>

#include "base/anim/controllable.h"
#include "base/anim/element.h"
#include "base/anim/options.h"

namespace Anim
{

class Group : public Controllable
{
public:
	using Index = size_t;

	void setBaseline();
	void resetBaseline();
	[[nodiscard]] Duration getBaseline() const;

	const Options &addElement(std::unique_ptr<IElement> element,
	    Options options);

	void reTime(Duration duration, Duration delay);

protected:
	void setPosition(Duration progress) override;
	void clear();

private:
	struct Record
	{
		Record(std::unique_ptr<IElement> element, Options options) :
		    element(std::move(element)),
		    options(std::move(options))
		{}
		std::unique_ptr<IElement> element;
		Options options;
	};

	std::vector<Record> elements;
	Duration baseline;

	void reTime(Options &options, Duration duration, Duration delay);
	void calcDuration(Duration duration);
};

}

#endif
