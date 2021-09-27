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
            title: 'Stacked Area Chart',
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
                    /* Making the chart elements fill the whole of
                       the y-axis as the default value is now 110% */
                    range: {
                        max: '100%' 
                    }
                }
            },
            title: 'Trellis Area Chart',
            split: true
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 2 (+)', 'Year'] },
                y: { detach: ['Value 2 (+)'] }
            },
            title: 'Bar Chart',
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
            /* Setting a custom rhythm for the animation
            to assist the viewer in following it. */
            geometry: { delay: 0, duration: 1 }, 
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