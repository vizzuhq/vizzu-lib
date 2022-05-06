#include "texttable.h"

#include <algorithm>

using namespace Vizzu;
using namespace Vizzu::Data;

void TextTable::setHeader(const Table::Header &header)
{
	Base::addHeader(header);
	textTypes.resize(header.size());
}

void TextTable::pushBackRow(const Table::Row &row)
{
	Base::addRow(row);
	for (auto i = 0u; i < std::min(header.size(), row.size()); i++)
	{
		textTypes[i].update(row[ColumnIndex(i)]);
	}
}

const TextTypes &TextTable::getTypes() const
{
	return textTypes;
}
