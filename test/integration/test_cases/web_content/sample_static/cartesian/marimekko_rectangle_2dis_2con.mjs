import { data } from '/test/integration/test_cases/web_content/content_data/chart_types_eu.js';

data.filter = record => 
record.Country == 'Bulgaria' || 
record.Country == 'Germany' || 
record.Country == 'Malta' || 
record.Country == 'Lithuania'

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'] },
                    x: { attach: ['Country', 'Value 2 (+)'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Country', 'Value 2 (+)'] }
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
                            format: 'categoriesFirst'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;