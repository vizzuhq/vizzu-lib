import { data } from '/test/integration/test_data/music_industry_history2.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Revenue', 'Format'] },
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
                        angle: 2.3,
                        fontSize: 11
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
                x: { set: ['Revenue', 'Year'] },
                y: { set: ['Format'] },
//                lightness: { set: ['Format (detailed)'] }
            },
            title: 'More Detailed',
            geometry: 'rectangle',
            split: false
        },
        style: {
            plot: {
                paddingLeft: 120,
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