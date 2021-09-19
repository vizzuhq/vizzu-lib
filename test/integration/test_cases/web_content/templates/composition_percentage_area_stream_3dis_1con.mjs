import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    [ 'AT', 'BE', 'CY', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ]
                    .includes(record.Country_code)
            }),
            config:
            {
                channels:
                {
                    x: { set: ['Year'] },
                    y: { set: ['Value 2 (+)', 'Country'], range: {min:'0%', max:'110%' } },
                    color: { set: ['Country'] },
                    size: { set: ['Country', 'Value 2 (+)'] }
                },
                title: 'Stacked Area Chart',
                geometry: 'area'
            }
        }
    ),
    chart => chart.animate(
        {            
            config:
            {
                title: 'Check relative differences between the elements',
                align: 'stretch'
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                title: '...or the elements separatelly',
                align: 'min',
                split:true
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                title: 'Area Chart',
                align: 'min',
                split:false
            }
        }
    )
];

export default testSteps;