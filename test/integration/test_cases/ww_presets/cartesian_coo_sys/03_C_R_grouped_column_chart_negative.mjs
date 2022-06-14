import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Austria' ||
                    record.Country == 'Belgium' ||
                    record.Country == 'Bulgaria' ||
                    record.Country == 'Cyprus' ||
     //               record.Country == 'Czechia' ||
     //               record.Country == 'Germany' ||
                    record.Country == 'Denmark' ||
     //               record.Country == 'Estonia' ||
     //               record.Country == 'Greece' ||
     //               record.Country == 'Spain' ||
     //               record.Country == 'Finland' ||
     //               record.Country == 'France' ||
     //               record.Country == 'Croatia' ||
                   record.Country == 'Hungary'
            }),
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Joy factors', 'Country'] },
                    color: { attach: 'Joy factors' },
                    label: { attach: 'Value 5 (+/-)' }
                },
                title: 'Grouped Column Chart with (-) Nums'

            },
            style:
            {
                plot: {
                    paddingLeft: 70,
                    yAxis: {
                        label: { paddingRight: 10 }
                        },
                    xAxis: {
                        label: { paddingTop: 10 }
                        }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;