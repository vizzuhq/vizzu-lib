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
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat', 'Positive nums'], range: '0,1.1,%' },
                    x: { attach: ['Country', 'Positive nums'] },
                    color: { attach: ['Threat'] },
                    label: { attach: ['Positive nums', 'Country'] }
                },
                title: 'Stacked Mekko chart'
            },
            style:
            {
                plot:
                {
                    paddingLeft: 140
                }
            }
        }
    )
];

export default testSteps;