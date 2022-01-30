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
                x: { set: ['Country', 'Value 2 (+)'] },
                y: {
                    set: ['Joy factors', 'Value 3 (+)'],
                    range: { max: '110%' }
                },
                color: { set: ['Joy factors'] },
                label: { set: ['Value 2 (+)', 'Country'] }
            },
            title: 'Stacked Mekko Chart',
            orientation: 'horizontal'
        }
    })
];

export default testSteps;