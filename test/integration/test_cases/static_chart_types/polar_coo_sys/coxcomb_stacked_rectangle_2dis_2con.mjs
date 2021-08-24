import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat', 'Positive nums'] },
                    x: { attach: ['Year', 'Positive nums'] },
                    color: { attach: ['Threat'] }
                },
                title: 'Stacked Coxcomb Chart',
                coordSystem: 'polar',
                legend: 'color'
            },
            style: 
            {
                plot: 
                {
                    marker: 
                    {
                        borderOpacity:1
                    } 
                }
            }
        }
    )
];

export default testSteps;