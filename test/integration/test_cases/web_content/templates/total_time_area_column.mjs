import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Year'] },
                    color: { attach: ['Joy factors'] },
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
                    y: { set: ['Value 2 (+)', 'Year']  },
                    x: { set: ['Joy factors'] }
                },
                title: 'Amount',
                geometry: 'rectangle',
                split: false
            }
        },
        {
            geometry: { delay: 0.43, duration: 1 },
            y: { delay: 1, duration: 2 },
            x: { delay: 0, duration: 1 }
        }
    ),
    chart => chart.animate(
    {
        config:
        {
            channels:
            {
                y: { detach: ['Year' ] },
                label: { set: ['Value 2 (+)' ] }
            }
        }
    }
)
];

export default testSteps;