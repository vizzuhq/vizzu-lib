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
                    record.Country == 'Denmark' ||
                    record.Country == 'Hungary'
            }),
            config: chart.constructor.presets.groupedBar({
                x: 'Value 5 (+/-)',
                y: 'Country',
                groupedBy: 'Joy factors',
                title: 'Grouped Bar Chart'
            })
        }
    )
];

export default testSteps;