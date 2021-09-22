import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: {
                    set: ['$exists', 'Value 2 (+)'],
                    range: { max: '130%' } // Set enought space for tangential labels.
                },
                label: { set: ['Value 2 (+)'] },
            },
            title: 'Polar Chart',
            geometry: 'area',
            coordSystem: 'polar',
        },
        style: { // This chart type no needs yAxis and yAxis-labels. 
            plot: {
                marker: {
                    label: {
                        orientation: 'tangential',
                        angle: 3.14 * -0.5
                    }
                },
                yAxis: {
                    title: { color: '#ffffff00' },
                    label: { color: '#ffffff00' }
                }
            }
        }
    })
];

export default testSteps;