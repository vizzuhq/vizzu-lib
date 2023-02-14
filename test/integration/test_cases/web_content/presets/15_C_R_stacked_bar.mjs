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
                    record.Country == 'Czechia'
            }),
            config: chart.constructor.presets.stackedBar({
                x: 'Value 2 (+)',
                y: 'Country',
                stackedBy: 'Joy factors',
                title: 'Stacked Bar Chart'
            })
        }
    )
];

export default testSteps;