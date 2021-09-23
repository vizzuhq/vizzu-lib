import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Joy factors'] },
                lightness: { set: ['Joy factors'] }, // Lightness channel not necessary but helps to fallow during the animation how spreading categories of the first chart.
                noop: { set: ['Year'] } //The noop channel splits the markers as all the other channels but will have no effect on the markers’ appearance. 
            },
            title: 'Distribution by Y',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 5 (+/-)'] },
                noop: { set: ['Joy factors'] }
            },
            title: 'Distribution by X'
        }
    })
];

export default testSteps;