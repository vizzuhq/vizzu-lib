#ifndef DATASET_TABLE_COLUMN_H
#define DATASET_TABLE_COLUMN_H

#include "types.h"
#include "series.h"
#include "value.h"
#include "dataset.h"

namespace Vizzu
{
namespace DataSet
{

class Column {
friend Table;
friend ColIterator;
public:
    const int pos;

    int size();
    ValueIterator begin() const;
    ValueIterator end() const;
    const SeriesInfo& info();

protected:
protected:
    TablePtr table;
    Column(const TablePtr& table, int pos);
};

class ColIterator {
friend class ColContainer;
public:
    int pos() const;
    const Column& operator*();
    ValueIterator operator++(int) const;
    ValueIterator operator--(int) const;

protected:
    int position;
    const ColContainer& container;
    ColIterator(const ColContainer& cnt, int pos);
};

class ColContainer {
friend class Table;
public:
    int size() const;
    ColIterator begin() const;
    ColIterator end() const;

protected:
    TablePtr table;
    ColContainer(const TablePtr& table);
};

}
}

#endif //DATASET_TABLE_COLUMN_H
