import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 2 (+)'] },
                y: {
                    set: ['Joy factors'],
                    range: { min: '-30%' } // Setting the radius of the empty circle in the centre.
                },
                color: { set: ['Joy factors'] },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Radial Bar Chart',
            coordSystem: 'polar'
        },
        style: { // All axes and axis labels are unnecessary on these types of charts, except for the labels of the y-axis.
            plot: {
                yAxis: {
                    color: '#ffffff00',
                    label: { paddingRight: 20 }
                },
                xAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' }
                }
            }
        }
    })
];

export default testSteps;