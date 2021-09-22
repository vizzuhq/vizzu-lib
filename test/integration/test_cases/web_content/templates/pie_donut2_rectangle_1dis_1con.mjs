import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 2 (+)'] },
                color: { set: ['Joy factors'] },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Pie Chart',
            coordSystem: 'polar'
        },
        style: {  // This chart type no needs axles and axis labels, except bigger size marker-labels. 
            plot: {
                marker: {
                    label: {
                        fontSize: 14,
                        position: 'center'
                    }
                },
                xAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' },
                    ticks: { color: '#ffffff00' }
                },
                yAxis: {
                    color: '#ffffff00',
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' },
                    ticks: { color: '#ffffff00' }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { range: { min: '-200%' } } // Set the radius of the empty center-circle. 
            },
            title: 'Donut Chart'
        }
    })
];

export default testSteps;