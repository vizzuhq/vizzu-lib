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
                    lightness: { attach: ['Country'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Size Continuous + Discrete.',
                legend: 'size',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    size: { attach: ['Country_code'] }
                }
            }
        }
    )
];

export default testSteps;