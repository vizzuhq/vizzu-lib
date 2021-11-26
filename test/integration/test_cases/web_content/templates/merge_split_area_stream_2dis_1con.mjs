import { data } from '/test/integration/test_data/music_industry_history_1.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Revenue [m$]', 'Format'] },
                color: { set: ['Format'] }
            },
            title: 'Stacked Streamgraph',
            geometry: 'area',
            align: 'center'
        },
        style: {
            plot: {
                xAxis: {
                    label: {
                        angle: 1.8,
                        fontSize: 8.5,
                        numberFormat: 'grouped'
                    }
                },
                yAxis: {
                    label: {
                        numberFormat: 'grouped'
                    }
                }
            }
        }
    }),
    chart => chart.animate( {
        config: {  
            channels: {
                y: { 
                    range: {
                        max: '100%'
                    }
                }
            },
            title: 'Trellis Area Chart',
            split: true,
            align: 'min'
        }
    }),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Revenue [m$]', 'Year'] },
                y: { set: ['Format'] }
            },
            title: 'Bar Chart',
            geometry: 'rectangle',
            split: false
        },
        style: {
            plot: {
                xAxis: {
                    label: {
                        angle: null,
                        fontSize: null
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
)
];

export default testSteps;