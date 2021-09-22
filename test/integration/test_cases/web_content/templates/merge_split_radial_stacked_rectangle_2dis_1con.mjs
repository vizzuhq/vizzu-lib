import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Country', 'Value 2 (+)'] },
                y: { 
                    set: ['Year'], 
                    range: { 
                        min: '-3' // Set the radius of the empty center-circle.
                    } 
                },
                color: { set: ['Country'] }
            },
            title: 'Stacked Radial Bar Chart',
            coordSystem: 'polar'
        },
        style: { // This chart type no needs axles and axis labels, except small size yAxis labels. 
            plot: {
                yAxis: {
                    color: '#ffffff00',
                    label: {
                        fontSize: 8
                    }
                },
                xAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            title: 'Splitted Radial Bar Chart',
            split: true
        }
    })
];

export default testSteps;