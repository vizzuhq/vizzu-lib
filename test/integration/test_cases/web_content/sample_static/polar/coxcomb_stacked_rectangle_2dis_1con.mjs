import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels:
            {
                y: {
                    attach: ['Joy factors', 'Value 2 (+)'],
                    range: {
                        min: '0%',
                        max: '107.5%'
                    }
                },
                x: { attach: ['Year'] },
                color: { attach: ['Joy factors'] }
            },
            title: 'Nightingale Chart',
            coordSystem: 'polar'
        },
        style: {
            plot: {
                yAxis: {
                    title: {
                        color: '#ffffff00'
                    },
                    label: {
                        color: '#ffffff00'
                    }
                },
                marker: {
                    rectangleSpacing: 0.1
                }
            }
        }
    })
];

export default testSteps;