import { data } from '../../../test_data/chart_types_eu.mjs';

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
            title: 'Bubble Plot',
            geometry: 'circle'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                /* With a dimension on the size channel Vizzu will
                stack the elements by the categories on the other channels
                on charts without coordinates. Here the Country code dimension is
                used to stack the bubbles by the dimension on the color channel. */
                size: { set: ['Value 2 (+)', 'Country_code'] } 
            },
            title: 'Stacked Bubble Chart'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                /* The stacking is eliminated when we remove the extra dimension
                from the size channel. */
                size: { set: ['Value 2 (+)'] } 
            },
            title: 'Bubble Chart'
        }
    })
];

export default testSteps;