import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                y: { set: ['Joy factors', 'Value 2 (+)'] },
                x: { set: ['Year'] },
                color: { set: ['Joy factors'] },
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        }
    }),
    chart => chart.animate({
        config: {
            split: true,
            title: 'Trellis Area Chart'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Value 2 (+)', 'Year'] },
                x: { set: ['Joy factors'] }
            },
            title: 'Column Chart',
            geometry: 'rectangle',
            split: false
        }
    },
        {
            /* Setting a custom rhythm for the animation
            to assist the viewer in following it. */
            geometry: {
                delay: 0.43,
                duration: 1
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