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
            })
        }
    )
];

export default testSteps;