import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                y: ['Joy factors', 'Value 2 (+)'],
                x: 'Year',
                color: 'Joy factors',
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        }
    }),

    chart => chart.animate({
        config: {
            title: 'Split Area Chart',
            split: true
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: ['Value 2 (+)', 'Year'],
                x: 'Joy factors'
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
                y: 'Value 2 (+)',
                x: 'Joy factors',
                label: 'Value 2 (+)'
            },
            title: 'Column Chart',
            geometry: 'rectangle',
            split: false
        }
    })
];

export default testSteps;