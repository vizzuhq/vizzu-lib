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
                    y: { set: 'Year'},
                    x: { set: 'Value 6 (+/-)'},
                    color: { set: 'Country'}
                },
                title: 'Line Chart vertical with (-)',
                geometry: 'line'
            },
            style: {
                plot: {
                    paddingLeft: '1.2em',
                    yAxis: {
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' },
                        title: { paddingTop: '2.4em' }
                        }
                    }
                }
            
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;