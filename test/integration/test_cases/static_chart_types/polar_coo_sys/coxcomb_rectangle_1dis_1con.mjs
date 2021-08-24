import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Positive nums'] },
                    x: { attach: ['Threat'] },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums'] }
                },
                title: 'Coxcomb Chart',
                orientation: 'horizontal',
                coordSystem: 'polar',
                legend: null
            },
            style:
            {
                data:{ columnMaxPadding: 0.347 },
                plot:
                { 
                    xAxis:                    
                    { label:
                        {
                            overflow: 'hidden' 
                        }
                    },
                    marker:
                    { label:
                        {
                            fontSize: 12,
                            position: 'center'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;