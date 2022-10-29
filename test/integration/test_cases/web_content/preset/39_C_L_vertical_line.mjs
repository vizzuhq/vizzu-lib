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
            config: chart.constructor.presets.line({
                x: 'Value 6 (+/-)',
                y: 'Year',
                dividedBy: 'Country',
                title: 'Vertical Line Chart'
            }),
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
    )
];

export default testSteps;