import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Threat', 'Positive nums'] },
                    y: { range: '1,-3,%' },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Donut Chart',
                legend: 'color',
                orientation: 'vertical',
                coordSystem: 'polar'
            },
            style:
            {
                plot:
                { marker:
                    { label:
                        {
                            position: 'top' 
                        }
                    },
                    xAxis:                    
                    { title:
                        {
                            side: 'positive',
                            paddingBottom: '25' 
                        }
                    }
                },
                data:
                { 
                    barMaxPadding: 0.2
                }
            }
        }

    )
];

export default testSteps;