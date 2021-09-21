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
                y: { set: ['Joy factors', 'Value 3 (+)'] },
                x: { set: ['Country', 'Value 2 (+)'] },
                color: { set: ['Joy factors'] },
                label: { set: ['Country'] },
                lightness: { set: ['Country'] }
            },
            title: 'Marimekko Chart',
            align: 'stretch'
        },
        style: {
            plot: {
                marker: {
                    label: {
                        format: 'dimensionsFirst'
                    }
                }
            }
        }
    }),
    chart => chart.animate({
        config: {
            orientation: 'vertical'
        }
    })
];

export default testSteps;