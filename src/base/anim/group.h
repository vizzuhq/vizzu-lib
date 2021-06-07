#ifndef ANIM_GROUP
#define ANIM_GROUP

#include <vector>
#include <memory>

#include "base/anim/controllable.h"
#include "base/anim/options.h"
#include "base/anim/element.h"

namespace Anim
{

class Group : public Controllable
{
public:
	void setBaseline();
	void resetBaseline();

	void addElement(
		std::unique_ptr<IElement> element,
	    Options options);

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
};

}

#endif
