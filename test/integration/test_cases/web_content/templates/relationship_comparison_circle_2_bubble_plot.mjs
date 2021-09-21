import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: {
                    set: ['Joy factors', 'Value 6 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                },
                y: {
                    set: ['Value 5 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                },
                color: { set: ['Joy factors'] },
                size: { set: ['Value 2 (+)'] },
                label: { set: ['Country_code'] }
            },
            title: 'Realationship',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                size: { set: ['Value 2 (+)', 'Country_code'] }
            },
            title: 'Stacked Comparison'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                size: { set: ['Value 2 (+)'] }
            },
            title: 'Comparison'
        }
    })
];

export default testSteps;