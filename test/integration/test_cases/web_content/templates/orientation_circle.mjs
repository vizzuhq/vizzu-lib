import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Joy factors'] },
                /* Lightness channel is used to assist the viewer
                in following the animation. */
                lightness: { set: ['Joy factors'] },
                /* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
                noop: { set: ['Year'] } 
            },
            title: 'Dot Plot',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 5 (+/-)'] },
                noop: { set: ['Joy factors'] }
            }
        }
    })
];

export default testSteps;