import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                ['AT', 'BE', 'CY', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE']
                    .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 2 (+)', 'Country'] },
                noop: { set: ['Country'] } //The noop channel splits the markers as all the other channels but will have no effect on the markers’ appearance.
            },
            title: 'Column Chart',
            geometry: 'rectangle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    range: {
                        max: '100%' // For use the whole range of the yAxis. (Default = 110%)  
                    }
                },
                color: { set: ['Country'] },
                noop: { set: null }
            },
            title: 'Drill down & check the elements separatelly',
            split: true,
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    range: {
                        max: 'auto' // Setting back the y-axis range to the default value.
                    }
                }
            },
            title: '...or together',
            split: false
        }
    }),
    chart => chart.animate({
        config: {
            title: '...or the relative differences between these',
            align: 'stretch'
        }
    })
];

export default testSteps;