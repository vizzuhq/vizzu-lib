"use strict";


const assert = (condition, message) => {
    if (!condition) {
        throw new Error('Assert failed: ' + (message || ''));
    }
};

export default class UnPivot {

    static isPivot(data) {
        return data.dimensions !== undefined
            || data.values !== undefined;
    }

    static convert(data) {
        assert('dimensions' in data, 'dimensions field is requreid');
        assert('values' in data, 'values field is requreid');
        assert(!('series' in data), 'both series and dimensions/values cannot be set');
        assert(!('records' in data), 'both series and dimensions/values cannot be set');

        data.series = [];

        let dimensionsProduct = 1;
        for (let i = 0; i < data.dimensions.length; i++) {
            let element = data.dimensions[i];
            assert('name' in element, 'name field is requreid');
            assert('values' in element, 'values field is requreid');
            assert(element.length != 0, 'length is zero');
            dimensionsProduct *= element.values.length;
        }

        let dimensionsBefore = 1;
        for (let i = 0; i < data.dimensions.length; i++) {
            let element = data.dimensions[i];
            let values = [];
            let valuesItem = [];
            element.values.forEach(value => {
                for (let b = 1; b <= dimensionsBefore; b++) {
                    valuesItem.push(value);
                }
            });
            dimensionsBefore *= element.values.length;
            for (let a = 1; a <= dimensionsProduct / dimensionsBefore; a++) {
                values = values.concat(valuesItem);
            }

            let seriesItem = {
                name: element.name,
                type: 'categories',
                values: values
            };
            data.series.push(seriesItem);
        }

        for (let i = 0; i < data.values.length; i++) {
            let element = data.values[i];
            assert('name' in element, 'name field is requreid');
            assert('values' in element, 'values field is requreid');

            let seriesItem = {
                name: element.name,
                type: 'values',
                values: data.values[i].values.flat(Infinity)
            };
            assert(seriesItem.values.length == dimensionsProduct, 'dimensions are not the same');
            data.series.push(seriesItem);
        }
    }
    
}