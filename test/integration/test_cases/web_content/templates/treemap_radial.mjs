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
                x: {
                    set: ['Value 2 (+)'],
                    range: {
                        max: '133%' // Set the openess of the polar coordinate system.
                    }
                },
                y: {
                    set: ['Joy factors'],
                    range: {
                        min: '-30%',// Set the radius of the empty center-circle.
                    }
                },
                size: { set: null },
                label: { set: ['Value 2 (+)'] }
            },
            title: 'Radial Chart',
            coordSystem: 'polar'
        },
        style: {  // This chart type no needs axles and axis labels, except yAxis-labels. 
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