import { data } from '/test/integration/test_data/chart_types_eu.js';


const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: ['Country'] },
            },
            title: 'Time distribution',
            geometry: 'area'
        },
        style: {
            tooltip: {
                layout: 'multiLine',
                dropShadow: '5',
                arrowSize: '8',
                seriesName: 'Country',
                borderRadius: '5'
            }
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: {
                    range: {
                        max: '100%' // For use the whole range of the yAxis. (Default = 110%)  
                    }
                }
            },
            split: true
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 2 (+)', 'Year'] },
                y: { detach: ['Value 2 (+)'] }
            },
            title: 'Total',
            geometry: 'rectangle',
            split: false,
        },
        style: {
            plot: {
                yAxis: {
                    label: {
                        paddingRight: 10,
                        fontSize: 10
                    }
                },
                marker: {
                    label: { fontSize: 10 }
                }
            }
        }
    },
        {
            geometry: { delay: 0, duration: 1 }, // Set timing for better animation.
            y: {
                delay: 0,
                duration: 1
            },
            x: {
                delay: 0,
                duration: 1
            }
        }
    ),
    chart => chart.animate({
        config: {
            channels: {
                x: { detach: ['Year'], },
                label: { attach: ['Value 2 (+)'] }
            }
        }
    }),
    chart => {
        chart.feature('tooltip', true);
        return chart;
    }
];

export default testSteps;