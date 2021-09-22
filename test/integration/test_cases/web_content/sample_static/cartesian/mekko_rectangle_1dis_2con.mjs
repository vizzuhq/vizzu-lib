import { data } from '/test/integration/test_data/chart_types_eu.js';

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
                x: { set: ['Country', 'Value 2 (+)'] },
                y: {
                    set: ['Value 1 (+)'],
                    range: { max: '110%' }
                },
                color: { set: ['Country'] },
                label: { set: ['Value 2 (+)', 'Country'] }
            },
            title: 'Mekko Chart'
        }
    }
    )
];

export default testSteps;