import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Value 6 (+/-)'] },
                /* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
                noop: { set: ['Joy factors'] },
                /* Lightness channel is used to assist the viewer
                in following the animation. */
                lightness: { set: ['Year'] }
            },
            title: 'Scatter Plot',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 6 (+/-)'] }
            },
            title: 'Dot Plot',
            legend: 'lightness'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Value 6 (+/-)'] }
            },
            title: 'Scatter Plot'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Joy factors'] },
                noop: { set: null }
            },
            title: 'Dot Plot'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Value 6 (+/-)'] },
                noop: { set: ['Joy factors'] },
            },
            title: 'Scatter Plot'
        }
    })
];

export default testSteps;