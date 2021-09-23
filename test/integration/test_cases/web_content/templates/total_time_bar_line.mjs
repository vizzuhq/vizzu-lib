import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                y: { set: ['Value 2 (+)'] },
                x: { set: ['Year'] },
                color: { attach: ['Joy factors'] }
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Joy factors'] },
                x: { set: ['Value 2 (+)', 'Year'] }
            },
            title: 'Bar Chart',
            geometry: 'rectangle',
            sort: 'byValue'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { detach: ['Year'] },
                label: { set: ['Value 2 (+)'] }
            }
        }
    })
];

export default testSteps;