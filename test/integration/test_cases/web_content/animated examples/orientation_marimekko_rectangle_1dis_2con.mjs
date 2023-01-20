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
                x: ['Country', 'Value 2 (+)'],
                y: ['Country', 'Value 3 (+)'],
                color: 'Country',
                label: 'Country',
                lightness: 'Country'
            },
            title: 'Marimekko Chart',
            align: 'stretch',
            orientation: 'horizontal'
        }
    }),

    chart => chart.animate({
        config: {
            title: 'Marimekko with Other Orientation',
            orientation: 'vertical'
        }
    })
];

export default testSteps;