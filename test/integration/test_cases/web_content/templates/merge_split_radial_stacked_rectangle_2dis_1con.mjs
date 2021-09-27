import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Country', 'Value 2 (+)'] },
                y: { 
                    set: ['Year'],
                    /* Setting the radius of the empty circle
                    in the centre. */
                    range: { 
                        min: '-3'
                    } 
                },
                color: { set: ['Country'] }
            },
            title: 'Radial Bar Chart',
            coordSystem: 'polar'
        },
        /* This chart type no needs axles and axis labels,
        except small size yAxis labels. */
        style: { 
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
            title: 'Trellis Radial Bar Chart',
            split: true
        }
    })
];

export default testSteps;