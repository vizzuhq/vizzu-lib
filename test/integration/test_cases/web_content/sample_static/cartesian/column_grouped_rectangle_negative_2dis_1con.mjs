import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Joy factors', 'Country'] },
                y: { set: ['Value 5 (+/-)'] },
                color: { set: ['Joy factors'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Grouped Column Chart'
        },
        // Labels have to be rotated on this chart.
        style: { 
            plot: {
                marker: {
                    label: {
                        fontSize: 6,
                        orientation: 'vertical',
                        angle: 3.14 * -1
                    }
                }
            }
        }
    })
];

export default testSteps;