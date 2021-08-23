import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { attach: ['Country'] },
                    label: { attach: ['Country_code'] },
                    lightness: { attach: ['Negative nums'] },
                    size: { attach: ['Negative nums'] }
                },
                title: 'Bubble Chart',
                geometry: 'circle',
                legend: null
            },
            style: 
            {
                plot: 
                {
                    marker: 
                    {
                        label: 
                        {
                            fontWeight: 'bold'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;