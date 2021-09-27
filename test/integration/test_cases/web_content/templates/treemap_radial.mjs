import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: { set: ['Joy factors'] },
                size: { set: ['Value 2 (+)'] },
                label: { set: ['Joy factors'] }
            },
            title: 'Treemap'
        },
        style: {
            plot: {
                marker: {
                    label: { fontSize: 14 }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 2 (+)'] },
                y: {
                    set: ['Joy factors'],
                    /* Setting the radius of
                    the empty circle in the centre. */
                    range: {
                        min: '-30%',
                    }
                },
                size: { set: null },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Radial Chart',
            coordSystem: 'polar'
        },
        /* All axes and axis labels are unnecessary on these types of charts,
        except for the labels of the y-axis. */
        style: {
            plot: {
                marker: {
                    label: { fontSize: null }
                },
                xAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' },
                    interlacing: { color: '#ffffff00' }
                },
                yAxis: {
                    color: '#ffffff00',
                    label: { paddingRight: 20 }
                }
            }
        }
    })
];

export default testSteps;