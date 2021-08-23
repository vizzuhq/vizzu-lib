import { data } from '/test/integration/test_data/chart_types_eu.js';


const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Neg_huge'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Color 2 Discrete series.',
                legend: 'color',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    color: { attach: ['Threat'] },
                    size: { attach: ['Threat'] },
                    label: { detach: ['Negative nums'] }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    size: { detach: ['Threat'] },
                    label: { attach: ['Negative nums'] }
                }
            }
        }
    )
];

export default testSteps;