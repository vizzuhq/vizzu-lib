import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Value 6 (+/-)'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Value 1 (+)'] },
                    label: { attach: ['Value 1 (+)'] }
                },
                title: 'Color legend On-Off Anim.',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    color: { detach: ['Country'] }
                }
            }
        }
    )
];

export default testSteps;