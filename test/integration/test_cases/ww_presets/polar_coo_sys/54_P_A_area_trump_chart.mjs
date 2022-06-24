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
                    record.Country == 'Germany' ||
                    record.Country == 'Denmark' ||
     //               record.Country == 'Estonia' ||
     //               record.Country == 'Greece' ||
                    record.Country == 'Spain' ||
                    record.Country == 'Finland' ||
                    record.Country == 'France' ||
     //               record.Country == 'Croatia' ||
                   record.Country == 'Hungary'
            }),
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: ['Country', 'Value 2 (+)'] },
                    color: { set: 'Country' }
                },
                title: 'Polar Area2 Chart',
                geometry: 'area',
                coordSystem: 'polar',
                align: 'max'
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    yAxis: {
                        label: {
                           paddingRight: '0em'
                        }
                    },
                    xAxis: {
                        label: {
                           paddingTop: '0.8em'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;