import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                y: {
                    set: ['Value 2 (+)'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
                x: { set: ['Year'] },
                color: { attach: ['Joy factors'] }
            },
            title: 'Time distribution',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Joy factors'], range: {min:'0%', max:'100%'} },
                x: { set: ['Value 2 (+)', 'Year'], range: {min:'0%', max:'110%'} }
            },
            title: 'Total',
            geometry: 'rectangle',
            sort: 'byValue'
        },
        style: {
            plot: {
                marker: {
                    borderOpacity: 1
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { detach: ['Year'] },
                label: { set: ['Value 2 (+)'] }
            }
        }
    })
];

export default testSteps;