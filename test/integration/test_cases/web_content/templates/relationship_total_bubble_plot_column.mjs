import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 6 (+/-)'] },
                y: { set: ['Value 5 (+/-)'] },
                color: { set: ['Joy factors'] },
                size: { set: ['Value 2 (+)'] },
                label: { set: ['Country_code'] }
            },
            title: 'Bubble Plot',
            geometry: 'circle'
        }
    }
    ),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Joy factors'] },
                x: { set: ['Value 2 (+)', 'Country_code'] },
                label: { set: null }
            },
            title: 'Bar Chart',
            geometry: 'rectangle',
            orientation: 'vertical'
        }
    },
        {
            geometry:
            {
                delay: 0.7,
                duration: 1
            }
        }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 2 (+)'] },
                label: { set: ['Value 2 (+)'] }
            }
        }
    })
];

export default testSteps;