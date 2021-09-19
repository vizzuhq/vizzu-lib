import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    [ 'AT', 'BE', 'DE', 'DK',  'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ]
                    .includes(record.Country_code)
            }),
            config:
            {
                channels:
                {
                    y: { set: ['Value 3 (+)', 'Country_code'] },
                    x: { set: ['Year', 'Joy factors'] },
                    color: { set: ['Country_code'] },
                    size: { set: ['Country_code', 'Value 2 (+)'] },
                    noop: { set: ['Year'] }
                },
                title: 'Stacked Streamgraph',
                geometry: 'area',
                align: 'center'
            },
            style: {
                plot: 
                {
                    marker: 
                    {
                        rectangleSpacing: 0.1,
                        borderWidth: 1,
                        borderOpacity: 1
                    }
                }
            }
        }
    )
];

export default testSteps;