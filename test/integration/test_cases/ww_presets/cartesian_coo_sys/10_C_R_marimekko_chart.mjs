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
                paddingLeft: '1.2em',
                yAxis: {
                    label: {
                       paddingRight: '0.8em'
                    }
                },
                xAxis: {
                  title: { paddingTop: '2.4em' },
                  label: { paddingTop: '0.8em' }
                }
            }
        }
    })
];

export default testSteps;