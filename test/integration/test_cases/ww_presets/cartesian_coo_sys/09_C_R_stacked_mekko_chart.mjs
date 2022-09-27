import { data } from '../../../test_data/chart_types_eu.mjs';

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
                    y: { attach: ['Joy factors', 'Value 2 (+)'] },
                    x: { attach: ['Country', 'Value 1 (+)'] },
                    color: { attach: 'Joy factors' },
                    label: { attach: 'Country' }
                },
                title: 'Stacked Mekko chart'
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;