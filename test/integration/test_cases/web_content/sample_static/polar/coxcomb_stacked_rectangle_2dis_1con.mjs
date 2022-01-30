import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Joy factors', 'Value 2 (+)'] },
                color: { set: ['Joy factors'] }
            },
            title: 'Nightingale (Coxcomb) Chart',
            coordSystem: 'polar'
        },
        /* y-axis and its labels are unnecessary
           on these types of charts. */
        style: { 
            plot: {
                yAxis: {
                    title: {
                        color: '#ffffff00'
                    },
                    label: {
                        color: '#ffffff00'
                    }
                },
                marker: {
                    rectangleSpacing: 0.1
                }
            }
        }
    })
];

export default testSteps;