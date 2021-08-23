import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Positive nums'] },
                },
                title: 'Treemap',
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