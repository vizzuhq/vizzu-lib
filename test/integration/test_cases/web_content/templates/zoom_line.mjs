import { data } from '/test/integration/test_data/chart_types_eu.js';

let filter1 = record =>
    [ 'AT', 'BE', 'DE', 'DK',  'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ].includes(record.Country_code);
let filter2 = record =>
    record.Year < 8 && record.Year > 2;
data.filter = filter1;

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { set: ['Year', 'Joy factors'] },
                    y: { set: ['Value 3 (+)'], range: {min:'0%', max:'110%'} },
                    color: { set: ['Country_code'] }
                },
                title: 'Zoom in',
                geometry: 'line'
            }
        }
    ),
    chart => chart.animate(
        {    
            data:{filter: record => filter1(record) && filter2(record) },
            config:
            {
                title: 'Zoomed in',
                align: 'min'
            }
        }
    )
];

export default testSteps;