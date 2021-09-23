import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Belgium'
    }),
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Year', 'Value 5 (+/-)'] },
                color: {
                    set: ['Value 5 (+/-)'],
                    range: { 
                        // Set range of colorGradient.
                        min: '-45',
                        max: '45'
                    }
                },
                noop: { set: ['Country'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Waterfall Chart',
            legend: 'color'
        },
        style: {
            plot: {
                marker: {
                    // Set the color palette of the colorGradient. 
                    colorGradient: 
                    [
                        '#3d51b8 0',
                        '#6389ec 0.15',
                        '#9fbffa 0.35',
                        '#d5d7d9 0.5',
                        '#f4b096 0.65',
                        '#e36c56 0.85',
                        '#ac1727 1'
                    ].join(),
                    label: {
                        position: 'top'
                    }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Value 5 (+/-)'] }
            },
            title: 'Column Chart'
        }
    })
];

export default testSteps;