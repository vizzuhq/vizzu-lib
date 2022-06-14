import { data } from '../../../test_data/chart_types_eu.mjs';

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
                y: { set: ['Joy factors', 'Value 3 (+)'] },
                color: { set: 'Joy factors' },
                label: { set: 'Country' },
                lightness: { set: 'Country' }
            },
            title: 'Marimekko Chart',
            align: 'stretch',
            orientation: 'horizontal'
        },
        style: {
            plot: {
                paddingLeft: 0,
                yAxis: {
                    label: {
                       paddingRight: 10
                    }
                },
                xAxis: {
                  title: { paddingTop: 30 },
                  label: { paddingTop: 10 }
                }
            }
        }
    })
];

export default testSteps;