import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 2 (+)'] },
                y: { range: { min: '-200%' } }, // Setting the radius of the empty circle in the centre.
                color: { set: ['Joy factors'] },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Donut Chart',
            coordSystem: 'polar'
        },
        style: { // All axes and axis labels are unnecessary on these types of charts. Meanwhile the marker labels are enlarged. 
            plot: {
                marker: {
                    label: {
                        fontSize: 14
                    }
                },
                xAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    ticks: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' }
                },
                yAxis: {
                    color: '#ffffff00',
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' }
                }
            }
        }
    })
];

export default testSteps;