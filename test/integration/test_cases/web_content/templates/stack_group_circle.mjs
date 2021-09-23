import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: { set: ['Joy factors'] },
                size: { set: ['Value 2 (+)'] },
                label: { set: ['Country_code'] }
            },
            title: 'Comparison',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                size: { set: ['Value 2 (+)', 'Country_code'] }
            },
            title: 'Stacked Comparison'
        },
        style: {
            plot: {
                marker: {
                    label: { fontSize: 10 } // Not a necessary step, makes it easier to read the labels on smaller circles.
                }
            }
        }
    })
];

export default testSteps;