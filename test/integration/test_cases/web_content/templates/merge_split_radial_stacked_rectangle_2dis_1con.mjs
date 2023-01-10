import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: ['Country', 'Value 2 (+)'],
                y: { set: 'Year',
                    /* Setting the radius of the empty circle
                    in the centre. */
                    range: { 
                        min: '-3'
                    } 
                },
                color: 'Country'
            },
            title: 'Radial Bar Chart',
            coordSystem: 'polar'
        },
        /* This chart needs small size yAxis labels. */
        style: { 
            plot: {
                yAxis: {
                    label: {
                        fontSize: 8
                    }
                }
            }
        }
    }),

    chart => chart.animate({
        config: {
            title: 'Trellis Radial Bar Chart',
            split: true
        }
    })
];

export default testSteps;