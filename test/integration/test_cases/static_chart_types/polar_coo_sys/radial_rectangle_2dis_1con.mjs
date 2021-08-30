
import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Joy factors'] },
                    x: { attach: ['Country', 'Value 2 (+)'], range: '0,1.33,%' },
                    color: { attach: ['Joy factors'] }
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