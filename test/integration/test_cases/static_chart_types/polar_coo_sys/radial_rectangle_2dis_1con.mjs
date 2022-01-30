
import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors'] },
                    x: { attach: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '133%' } },
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