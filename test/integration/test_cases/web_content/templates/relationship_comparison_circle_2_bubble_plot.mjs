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
            title: 'Relationship',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                size: { set: ['Value 2 (+)', 'Country_code'] } // Put dimension data series on 'size' channel to stack by other dimension which is on 'color' channel. 
            },
            title: 'Stacked Comparison'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                size: { set: ['Value 2 (+)'] } // Take dimension data series off from 'size' channel to terminate stacking. 
            },
            title: 'Comparison'
        }
    })
];

export default testSteps;