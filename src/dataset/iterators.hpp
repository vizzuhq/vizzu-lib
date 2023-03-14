/**
 * Implementation of ValueIterator class
 */
template<class S>
IndexBasedIterator<S>::IndexBasedIterator()
    : index(0)
{
}

template<class S>
IndexBasedIterator<S>::IndexBasedIterator(int index, S series)
    : index(index), series(series)
{
    if (index < 0)
        index = 0;
    else if (index > series->size())
        index = series->size();
}

template<class S>
IndexBasedIterator<S>& IndexBasedIterator<S>::operator++() {
    if (index == series->size())
        index = 0;
    else
        index++;
    return *this;
}

template<class S>
IndexBasedIterator<S> IndexBasedIterator<S>::operator++(int) {
    IndexBasedIterator temp(index, series);
    operator++();
    return temp;
}

template<class S>
IndexBasedIterator<S>& IndexBasedIterator<S>::operator--() {
    if (index == 0)
        index = series->size();
    else
        index--;
    return *this;
}

template<class S>
IndexBasedIterator<S> IndexBasedIterator<S>::operator--(int) {
    IndexBasedIterator temp(index, series);
    operator--();
    return temp;
}

template<class S>
IndexBasedIterator<S>& IndexBasedIterator<S>::operator+=(int arg) {
    index += arg;
    if (index < 0)
        index = 0;
    else if (index > series->size())
        index = series->size();
    return *this;
}

template<class S>
IndexBasedIterator<S> IndexBasedIterator<S>::operator+(int arg) const {
    return IndexBasedIterator{index + arg, series};
}

template<class S>
IndexBasedIterator<S>& IndexBasedIterator<S>::operator-=(int arg) {
    index -= arg;
    if (index < 0)
        index = 0;
    else if (index > series->size())
        index = series->size();
    return *this;
}

template<class S>
IndexBasedIterator<S> IndexBasedIterator<S>::operator-(int arg) const {
    return IndexBasedIterator{index - arg, series};
}

template<class S>
bool IndexBasedIterator<S>::operator==(const IndexBasedIterator& arg) const {
    return index == arg.index && series == arg.series;
}

template<class S>
bool IndexBasedIterator<S>::operator!=(const IndexBasedIterator& arg) const {
    return index != arg.index || series != arg.series;
}

template<class S>
Value IndexBasedIterator<S>::operator*() const {
    return series->valueAt(index);
}

template<class S>
IndexBasedIterator<S>::operator int() {
    return index;
}

template<class S>
Value IndexBasedIterator<S>::value() const {
    return series->valueAt(index);
}

template<class S>
ValueType IndexBasedIterator<S>::type() const {
    return series->typeAt(index);
}
