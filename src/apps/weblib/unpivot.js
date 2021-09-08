"use strict";


const assert = (condition, message) => {
    if (!condition) {
        throw Error('Assert failed: ' + (message || ''));
    }
};


export default class UnPivot {

    convert(input) {
        assert('categories' in input, 'categories field is requreid');
        assert('values' in input, 'values field is requreid');

        let series = { series: [] };

        let dimensionsProduct = 1;
        for (let i = 0; i < input.categories.length; i++) {
            let element = input.categories[i];
            assert('name' in element, 'name field is requreid');
            assert('values' in element, 'values field is requreid');
            assert(element.length != 0, 'length is zero');
            dimensionsProduct *= element.values.length;
        }

        let dimensionsBefore = 1;
        for (let i = 0; i < input.categories.length; i++) {
            let element = input.categories[i];
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
            series.series.push(seriesItem);
        }

        for (let i = 0; i < input.values.length; i++) {
            let element = input.values[i];
            assert('name' in element, 'name field is requreid');
            assert('values' in element, 'values field is requreid');

            let seriesItem = {
                name: element.name,
                type: 'values',
                values: input.values[i].values.flat(Infinity)
            };
            assert(seriesItem.values.length == dimensionsProduct, 'dimensions are not the same');
            series.series.push(seriesItem);
        }

        return series;
    }
    
}