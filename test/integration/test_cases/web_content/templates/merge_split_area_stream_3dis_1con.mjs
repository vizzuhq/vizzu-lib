import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record =>
    [ 'AT', 'BE', 'DE', 'DK',  'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ].includes(record.Country_code);

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { set: ['Year', 'Joy factors'] },
                    y: { set: ['Value 3 (+)', 'Country'] },
                    color: { set: ['Country'] },
                    size: { set: ['Country', 'Value 2 (+)'] }
                },
                title: 'Stacked Streamgraph',
                geometry: 'area',
                align: 'center'
            }
        }
    ),
    chart => chart.animate(
        {            
            config:
            {
                title: 'Check the elements separatelly',
                split: true,
                align: 'min'
            }
        }
    )
];

export default testSteps;