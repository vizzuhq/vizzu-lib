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
     //               record.Country == 'Denmark' ||
     //               record.Country == 'Estonia' ||
     //               record.Country == 'Greece' ||
     //               record.Country == 'Spain' ||
     //               record.Country == 'Finland' ||
     //               record.Country == 'France' ||
     //               record.Country == 'Croatia' ||
                   record.Country == 'Hungary'
            }),
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: 'Value 2 (+)' },
                    color: { set: 'Country' }
                },
                title: 'Polar Overlay Area Chart',
                geometry: 'area',
                coordSystem: 'polar',
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    marker: {
                        fillOpacity: 0.5,
                        borderWidth: 0},
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