import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Austria' ||
                    record.Country == 'Belgium' ||
                    record.Country == 'Bulgaria' ||
                    record.Country == 'Cyprus' ||
                    record.Country == 'Denmark' ||
                    record.Country == 'Estonia' ||
                    record.Country == 'Greece' ||
                    record.Country == 'Spain' ||
                    record.Country == 'Hungary'
            }),
            config: chart.constructor.presets.heatmap({
                x: 'Year',
                y: 'Country_code',
                lightness: 'Value 3 (+)',
                title: 'Heatmap'
            }),
            style: {
                plot: {
                    marker: {
                        rectangleSpacing: 0
                    }
                }
            }
        }
    )
];

export default testSteps;