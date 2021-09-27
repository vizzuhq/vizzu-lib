import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: {
                    set: ['$exists', 'Value 2 (+)'],
                    /* Set enough space for 
                    tangential labels. */
                    range: { max: '130%' }
                },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Polar Line Chart',
            geometry: 'line',
            coordSystem: 'polar'
        },
        style: {
            plot: {
                marker: {
                    label: {
                        orientation: 'tangential',
                        angle: 3.14 * -0.5
                    }
                },
                /* y-axis and its labels are unnecessary
                on these types of charts. */
                yAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' }
                }
            }
        }
    })
];

export default testSteps;