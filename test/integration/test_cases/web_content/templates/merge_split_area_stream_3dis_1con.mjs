import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
            [ 'AT', 'BE', 'DE', 'DK', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE' ]
            .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: { set: ['Year', 'Joy factors'] },
                y: { 
                    set: ['Value 3 (+)', 'Country'],
                    range: {
                        min: '0%',
                        max: '110%'
                    }
                },
                color: { set: ['Country'] },
                size: { set: ['Country', 'Value 2 (+)'] }
            },
            title: 'Stacked Streamgraph',
            geometry: 'area',
            align: 'center'
        }
    }),
    chart => chart.animate(
        {
            config: {
                channels: {
                    y: { 
                        range: {
                            min: '0%',
                            max: '100%'
                        }
                    }
                },
                title: 'Check the elements separatelly',
                split: true,
                align: 'min'
            }
        })
];

export default testSteps;