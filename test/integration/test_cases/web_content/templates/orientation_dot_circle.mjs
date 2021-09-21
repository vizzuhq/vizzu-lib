import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: {
                    set: ['Value 5 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                },
                y: {
                    set: ['Value 6 (+/-)'],
                    range: {
                        min: '-10%',
                        max: '110%'
                    }
                },
                noop: { set: ['Joy factors'] },
                lightness: { set: ['Year'] }
            },
            title: 'Relationship by X, Y',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 6 (+/-)'] }
            },
            title: 'Distribution by X',
            legend: 'lightness'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Value 6 (+/-)'] }
            },
            title: 'Relationship by X, Y'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Joy factors'] },
                noop: { set: null }
            },
            title: 'Distribution by Y'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Value 6 (+/-)'] },
                noop: { set: ['Joy factors'] },
            },
            title: 'Relationship by X, Y'
        }
    })
];

export default testSteps;