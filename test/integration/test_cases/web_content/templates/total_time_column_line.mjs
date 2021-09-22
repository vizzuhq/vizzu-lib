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
            title: 'Time distribution',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Value 2 (+)', 'Year'] },
                x: { set: ['Joy factors'] }
            },
            title: 'Amount',
            geometry: 'rectangle',
            sort: 'byValue'
        }
    },
        {
            geometry: { // Set timing for better animation.
                delay: 0.4,
                duration: 0.8
            },
            y: {
                delay: 1,
                duration: 2
            },
            x: {
                delay: 0,
                duration: 1
            }
        }
    ),
    chart => chart.animate({
        config: {
            channels: {
                y: { detach: ['Year'] },
                label: { set: ['Value 2 (+)'] }
            }
        }
    })
];

export default testSteps;