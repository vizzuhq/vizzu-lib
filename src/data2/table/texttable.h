#ifndef TEXTTABLE_H
#define TEXTTABLE_H

#include <string>

#include "table.h"
#include "texttype.h"

namespace Vizzu
{
namespace Data
{

class TextTable : public Table<std::string>
{
public:
	void setHeader(const Header &header);
	void pushBackRow(const Row &row);
	const TextTypes &getTypes() const;

private:
	typedef Table<std::string> Base;

	TextTypes textTypes;
};

}
}

#endif
