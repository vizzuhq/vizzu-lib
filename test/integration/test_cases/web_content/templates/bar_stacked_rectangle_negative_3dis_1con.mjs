import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record =>
    [ 'AT', 'BE', 'DE', 'DK',  'ES'/*, 'FI', 'FR', 'IT', 'NL', 'SE'*/ ].includes(record.Country_code);

const testSteps = [
    chart => chart.animate(
        {
            data: data,


            descriptor:
            {
                channels:
                {
                    x: { set: ['Value 3 (+)', 'Country_code'] },
                    y: { set: ['Year', 'Joy factors'] },
                    color: { set: ['Country_code'] },
                    size: { set: ['Country_code', 'Value 2 (+)'] },
                    noop: { set: ['Year'] }
                },
                title: 'Stacked Bar Chart (Amount of...)'
            },
            style: {
                data: 
                { 
                    barMaxPadding: 0.07 
                },
                plot: 
                {
                    marker: 
                    {
                        borderWidth: 1
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {            
            descriptor:
            {
                title: 'Check the elements separatelly',
                split: true
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor:
            {
                title: 'Stack them again',
                split: false
            }
        }
    )
];

export default testSteps;