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
                    record.Country == 'Germany' ||
                    record.Country == 'Denmark' ||
                    record.Country == 'Estonia' ||
                    record.Country == 'Spain' ||
                    record.Country == 'France' 
            }),
            config:
            {
                channels:
                {
                    x: { attach: 'Year'},
                    y: { attach: 'Value 6 (+/-)'},
                    color: { attach: 'Country'}
                },
                title: 'Line Chart with (-)',
                geometry: 'line'
            },
            style: {
                plot: {
                    paddingLeft: 0,
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