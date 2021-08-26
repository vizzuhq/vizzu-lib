
import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat'] },
                    x: { attach: ['Country', 'Positive nums'], range: '0,1.33,%' },
                    color: { attach: ['Threat'] }
                },
                title: 'Radial Chart (2disc)',
                coordSystem: 'polar',
                orientation: 'vertical',
                align: 'min'
            },
            style: 
            {
                plot: 
                {
                    yAxis: 
                    { 
                        label: 
                        { paddingRight: 20
                        } 
                    },
                    marker: 
                    {
                       borderOpacity: 1 
                    }
                }
            }
        }
    )
];

export default testSteps;