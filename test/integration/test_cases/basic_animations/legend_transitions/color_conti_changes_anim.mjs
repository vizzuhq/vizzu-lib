import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Value 6 (+/-)'], range: '0,1.1,%' },
                    color: { attach: ['Value 4 (+/-)'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Country'] }
                },
                title: 'Color Continuous series Changing Anim.',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    color: { detach: ['Value 4 (+/-)'], attach: ['Value 5 (+/-)'] }
                }
            }
        }
    )
];

export default testSteps;