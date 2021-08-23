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
                    color: { attach: ['Negative nums'] },
                    lightness: { attach: ['Country'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Lightness Discrete Anim.',
                legend: 'lightness',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            data: {
                filter: record => 
                record.Country != 'Germany' && 
                record.Country != 'Italy' && 
                record.Country != 'Hungary' && 
                record.Country != 'Netherlands',
            },
            descriptor: {
                title: 'Lightness Discrete Anim. - Filtered'
            }
        }
    )
];

export default testSteps;