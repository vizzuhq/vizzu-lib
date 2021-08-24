import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { attach: ['Threat'] },
                    size: { attach: ['Country_code', 'Positive nums'] },
                    label: { attach: ['Country_code'] }
                },
                title: 'Bubble chart',
                geometry: 'circle',
                legend: 'color'
            },
            style: 
            {
                plot: 
                {
                    marker: 
                    {
                        label: 
                        {
                            fontSize: 9
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;