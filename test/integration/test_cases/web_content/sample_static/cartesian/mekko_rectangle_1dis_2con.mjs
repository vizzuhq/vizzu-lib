import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Bulgaria' ||
                record.Country == 'Germany' ||
                record.Country == 'Malta' ||
                record.Country == 'Lithuania'
        }),
        config: {
            channels: {
                x: ['Country', 'Value 2 (+)'],
                y: { set: ['Value 1 (+)'], range: { max: '110%' } },
                color: 'Country',
                label: ['Value 2 (+)', 'Country']
            },
            title: 'Mekko Chart'
        }
    }
    )
];

export default testSteps;