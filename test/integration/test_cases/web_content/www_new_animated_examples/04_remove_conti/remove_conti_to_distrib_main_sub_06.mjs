import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: 'Value 5 (+/-)',
                y: 'Value 6 (+/-)',
                /* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
                noop: 'Joy factors',
                /* Lightness channel is used to assist the viewer
                in following the animation. */
                lightness: 'Year'
            },
            title:'Title',
            geometry: 'circle'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year'
            },
            title:'Title',
            orientation: 'horizontal'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Value 5 (+/-)'
            },
            title:'Title'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: 'Joy factors',
                noop: null 
            },
            title:'Title',
            orientation: 'vertical'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: 'Value 6 (+/-)',
                noop: 'Joy factors'
            },
            title:'Title'
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;