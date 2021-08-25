import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat', 'Positive nums'], range: '0,1.075,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Threat'] }
                },
                title: 'Stacked Coxcomb Chart',
                coordSystem: 'polar'
            },
            style: 
            {
                data: 
                { 
                    columnMaxPadding: 0.21 
                },
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