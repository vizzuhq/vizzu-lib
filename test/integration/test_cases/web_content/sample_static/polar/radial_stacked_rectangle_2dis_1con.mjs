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
                    range: { min: '-20%' }
                },
                color: { set: ['Country'] },
                lightness: { set: ['Year'] }
            },
            title: 'Stacked Radial Bar Chart',
            coordSystem: 'polar'
        },
        /* This chart type no needs axles and axis labels,
        except smaller yAxis-labels. */
        style: { 
            plot: {
                yAxis: {
                    color: '#ffffff00',
                    label: { fontSize: 8 }
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