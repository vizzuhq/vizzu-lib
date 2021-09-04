import { data } from '/test/integration/test_data/chart_types_eu.js';

data.filter = record =>
    [ 'AT', 'BE', 'CY', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ].includes(record.Country_code);

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { set: ['Year'] },
                    y: { set: ['Value 2 (+)', 'Country']/*, range: '0,1.1,%'*/ },
                    noop: { set: ['Country'] }
                },
                title: 'Column Chart',
                geometry: 'rectangle'
            }
        }
    ),
    chart => chart.animate(
        {            
            config:
            {
                channels:
                {
                    color: { set: ['Country'] },
                    noop: { set: null }
                },
                title: 'Drill down & check the elements separatelly',
                split:true,
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                title: '...or together',
                align: 'min',
                split:false
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {                
            title: '...or the relative differences between these',
            align: 'stretch',
            split:false
            }
        }
    )
];

export default testSteps;