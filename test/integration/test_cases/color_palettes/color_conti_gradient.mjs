import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Negative nums'], range: '0,1.5,%' },
                    x: { attach: ['Neg_huge'], range: '0,1.5,%' },
                    color: { attach: ['Negative nums'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] },
                    noop: { attach: ['Country'] } 
                },
                title: 'Default Color Gradient',
                legend: 'color',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;