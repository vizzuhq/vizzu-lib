import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Bulgaria' ||
                    record.Country == 'Germany' ||
                    record.Country == 'Malta' ||
                    record.Country == 'Lithuania'
            }),
            config: chart.constructor.presets.mekko({
                x: 'Value 1 (+)',
                y: 'Value 2 (+)',
                stackedBy: 'Joy factors',
                groupedBy: 'Country',
                title: 'Stacked Mekko Chart'
            })
        }
    )
];

export default testSteps;