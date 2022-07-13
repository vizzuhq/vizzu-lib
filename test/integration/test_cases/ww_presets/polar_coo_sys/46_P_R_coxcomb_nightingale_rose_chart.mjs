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
            config:
            {
                channels:
                {
                    x: { attach: ['Country'] },
                    y: { attach: ['Value 2 (+)'] },
                    color: { attach: 'Joy factors' }
                },
                title: 'Coxcomb, Nightingale, Rose',
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    paddingLeft:'0em',
                    paddingRight:'12.42em',
                    marker: {
                        rectangleSpacing: '0',
                        fillOpacity: 0.5,
                        borderWidth: 0,
                    },
                    xAxis: {
                        title: { color: '#ffffff00' }
                    },
                    yAxis:
                    {
                        color: '#00000000',
                        title: { color: '#ffffff00' },
                        label: { color: '#ffffff00' },
                        ticks: { color: '#ffffff00' },
//                        interlacing: { color: '#ffffff00' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;