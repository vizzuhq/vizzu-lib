import { data } from '/test/integration/test_data/chart_types_eu.js';


const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                y: {
                    set: ['Country', 'Value 2 (+)'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
                x: { set: ['Year'] },
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
                        min: '0%',
                        max: '100%'
                    }
                }
            },
            split: true
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                y: { detach: ['Value 2 (+)'] },
                x: {
                    set: ['Value 2 (+)', 'Year'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                }
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
                }
            }
        }
    },
        {
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
                x: {
                    detach: ['Year'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
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