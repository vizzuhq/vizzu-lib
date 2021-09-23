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
                size: { set: ['Value 2 (+)', 'Country_code'] } // With a dimension on the size channel Vizzu will stack the elements by the categories on the other channels on charts without coordinates. Here the Country code dimension is used to stack the bubbles by the dimension on the color channel. 
            },
            title: 'Stacked Comparison'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                size: { set: ['Value 2 (+)'] } // The stacking is eliminated when we remove the extra dimension from the size channel. 
            },
            title: 'Comparison'
        }
    })
];

export default testSteps;