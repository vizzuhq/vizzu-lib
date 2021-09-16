import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record => 
record.Country == 'Bulgaria' || 
record.Country == 'Germany' || 
record.Country == 'Malta' || 
record.Country == 'Lithuania'

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 3 (+)'] },
                    x: { attach: ['Country', 'Value 2 (+)'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Country'] },
                    lightness: { attach: ['Country'] }
                },
                title: 'Marimekko Chart',
                align: 'stretch'
            },
            style:
            {
                plot:
                {
                    marker:
                    {
                        label:
                        {
                            format: 'dimensionsFirst'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                orientation: 'vertical'
            }
        }
    )
];

export default testSteps;