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
                    record.Country == 'Czechia' ||
     //               record.Country == 'Germany' ||
                    record.Country == 'Denmark' 
     //               record.Country == 'Estonia' ||
     //               record.Country == 'Greece' ||
     //               record.Country == 'Spain' ||
     //               record.Country == 'Finland' ||
     //               record.Country == 'France' ||
     //               record.Country == 'Croatia' ||
     //              record.Country == 'Hungary'
            }),
            config:
            {
                channels:
                {
                    y: { attach: 'Country', range: { min: '-50%' }  },
                    x: { attach: ['Joy factors', 'Value 2 (+)']},
                    color: { attach: 'Joy factors' }
                },
                title: 'Stacked Radial Bar Chart',
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    yAxis: {                
                        color: '#ffffff00',
                        title: { color: '#ffffff00' },
                        ticks: { color: '#ffffff00' },
                        label: { paddingRight: '0.8em' },
                    },
                    xAxis: {
                        title: { paddingTop: '2.4em' },
                        interlacing: { color: '#ffffff00' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;