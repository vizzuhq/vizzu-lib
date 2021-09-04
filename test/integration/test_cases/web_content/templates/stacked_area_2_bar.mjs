import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                },
                title: 'Time distribution',
                geometry: 'area'
            }
        }
    ),
    chart => chart.animate(
        {
            config: 
            {
                split: true
            }
        }
    ),
        chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { detach: ['Value 2 (+)'], range: { min: '0%', max: '100%' }  },
                    x: { attach: ['Value 2 (+)' ] }
                },
                title: 'Amount',
                geometry: 'rectangle',
                split: false
            },
            style: {
                plot: {
                    marker: {
                        borderOpacity: 1,

                    }
                }
            }
        },
        {
//            style: { delay: 0, duration: 2 },
//            title: { delay: 0, duration: 2 },
//            legend: { delay: 0, duration: 2 },
//            show: { delay: 0, duration: 2 },
//            hide: { delay: 0, duration: 2 },
//            color: { delay: 0, duration: 2 },
//            coordSystem: { delay: 2, duration: 2 },
            geometry: { delay: 0, duration: 2 },
            y: { delay: 0, duration: 2 },
            x: { delay: 0, duration: 2 }
        }
    )
];

export default testSteps;