import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Country'] },
                y: { set: ['Joy factors', 'Value 2 (+)'] },
                color: { set: ['Joy factors'] },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Stacked Column Chart'
        },
        style: { // Labels have to be rotated on this chart.
            plot: {
                xAxis: {
                    label: {
                        angle: 2.3,
                        fontSize: 11
                    }
                },
                marker: {
                    label: {
                        fontSize: 7,
                        orientation: 'vertical',
                        angle: 3.14 * -1
                    }
                }
            }
        }
    })
];

export default testSteps;