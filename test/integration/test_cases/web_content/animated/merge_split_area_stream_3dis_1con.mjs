import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                ['AT', 'BE', 'DE', 'DK', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE']
                    .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: ['Year', 'Joy factors'],
                y: ['Value 3 (+)', 'Country'],
                color: 'Country'
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
                        /* Making the chart elements fill the whole of
                        the y-axis as the default value is now 110% */
                        range: {
                            max: '100%' 
                        }
                    }
                },
                title: 'Split Area Chart',
                split: true,
                align: 'min'
            }
        })
];

export default testSteps;