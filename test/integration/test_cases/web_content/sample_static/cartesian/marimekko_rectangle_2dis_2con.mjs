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
                y: { attach: ['Joy factors', 'Value 2 (+)'] },
                x: { attach: ['Country', 'Value 2 (+)'] },
                color: { attach: ['Joy factors'] },
                label: { attach: ['Country', 'Value 2 (+)'] }
            },
            title: 'Marimekko Chart',
            align: 'stretch'
        },
        style: {
            plot: {
                marker: {
                    label: {
                        format: 'dimensionsFirst',
                        fontSize: '8.5'
                    }
                }
            }
        }
    })
];

export default testSteps;