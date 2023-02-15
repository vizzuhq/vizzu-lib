import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
            record.Country == 'Austria' ||
            record.Country == 'Belgium' ||
            record.Country == 'Bulgaria' ||
            record.Country == 'Cyprus' ||
            record.Country == 'Czechia' ||
            record.Country == 'Denmark'
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