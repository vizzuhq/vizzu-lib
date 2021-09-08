"use strict";


const assert = (condition, message) => {
    if (!condition) {
        throw new Error('Assert failed: ' + (message || ''));
    }
};


const assertArray = (data, array, index) => {
    assert(Array.isArray(array), 'array is not a list');
    try {
        if (array.length !== data.dimensions[index].values.length) {
            throw new Error('Assert failed');
        }
    } catch (err) {
        assert(false, 'dimensions are not the same');
    }
    array.forEach(item => {
        if (Array.isArray(item)) {
            assertArray(data, item, index - 1);
        }
    });
};


export default class UnPivot {

    static isPivot(data) {
        return data.dimensions !== undefined
            || data.measures !== undefined;
    }

    static convert(data) {
        assert(typeof data === 'object' && data !== null && !Array.isArray(data), 'data is not an object');
        assert('dimensions' in data, 'dimensions field is requreid');
        assert('measures' in data, 'measures field is requreid');
        assert(!('series' in data), 'both series and dimensions/measures cannot be set');

        data.series = [];

        let dimensionsProduct = 1;
        assert(Array.isArray(data.dimensions), 'data.dimensions is not a list');
        for (let i = 0; i < data.dimensions.length; i++) {
            let item = data.dimensions[i];
            assert(typeof item === 'object' && item !== null && !Array.isArray(item), 'item is not an object');
            assert('name' in item, 'name field is requreid');
            assert(item.type === undefined || item.type === 'categories', 'item.type is invalid');
            assert('values' in item, 'values field is requreid');
            assert(Array.isArray(item.values), 'item.values is not a list');
            assert(item.values.length !== 0, 'length is zero');
            dimensionsProduct *= item.values.length;
        }

        let dimensionsBefore = 1;
        for (let i = 0; i < data.dimensions.length; i++) {
            let item = data.dimensions[i];
            let values = [];
            let valuesItem = [];
            item.values.forEach(value => {
                for (let b = 1; b <= dimensionsBefore; b++) {
                    valuesItem.push(value);
                }
            });
            dimensionsBefore *= item.values.length;
            for (let a = 1; a <= dimensionsProduct / dimensionsBefore; a++) {
                values = values.concat(valuesItem);
            }

            let seriesItem = {
                name: item.name,
                type: item.type || 'categories',
                values: values
            };
            data.series.push(seriesItem);
        }

        assert(typeof data.measures === 'object' && data.measures !== null, 'data.measures is not a list or an object');
        if (!Array.isArray(data.measures)) {
            data.measures = [ data.measures ];
        }
        for (let i = 0; i < data.measures.length; i++) {
            let item = data.measures[i];
            assert(typeof item === 'object' && item !== null && !Array.isArray(item), 'item is not an object');
            assert('name' in item, 'name field is requreid');
            assert(item.type !== 'categories', 'item.type is invalid');
            assert('values' in item, 'values field is requreid');
            assert(Array.isArray(item.values), 'item.values is not a list');
            assertArray(data, item.values, data.dimensions.length - 1);
            let seriesItem = {
                name: item.name,
                type: item.type,
                values: item.values.flat(Infinity)
            };
            if (seriesItem.type === undefined) {
                delete seriesItem.type;
            }
            assert(seriesItem.values.length === dimensionsProduct, 'dimensions are not the same');
            data.series.push(seriesItem);
        }
    }
    
}