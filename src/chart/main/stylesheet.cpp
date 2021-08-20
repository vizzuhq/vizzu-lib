#include "stylesheet.h"

using namespace Vizzu;
using namespace Vizzu::Styles;

Chart Sheet::getFullParams(const Diag::DiagramOptionsPtr &options)
{
	this->options = options.get();

	calcDefaults();

	auto params = Base::getFullParams();

	calcFinals(params);

	return params;
}

void Sheet::calcDefaults()
{

}

void Sheet::calcFinals(Chart &/*params*/)
{

}
