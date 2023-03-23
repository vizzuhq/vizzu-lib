import { data } from '../../../../test_data/music_industry_history_1.mjs';

const testSteps = [

    chart => {
        chart.on('plot-axis-label-draw', event => {
            let year = parseFloat(event.data.text);
            if (!event.data.text.includes('$') && !isNaN(year) && year % 5 != 0)
                event.preventDefault();
        });
            return chart
    },

    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Format == 'Tapes'
        }),
        config: {
            channels: {
                x: { set: 'Year', range: { max: '48'} }, // 48 = years in this time period
                y: ['Revenue [$]', 'Format'],
                color: 'Format'
            },
            title: 'Title',
            geometry: 'area',
            align: 'center'
        },
        style:{
            plot:{
                yAxis:{
                    label:{
                        numberScale: 'K, M, B, T'
                    }
                }
            }
        }
    }),

    chart => chart.animate({
        data: {
            filter: record =>
                record.Format == 'Tapes' ||
                record.Format == 'Vinyl'
        },
        config: {
            title: 'Title'
        }
    }),

    chart => chart.animate({
        data: {
            filter: record =>
                record.Format == 'Tapes' ||
                record.Format == 'Cassette' ||
                record.Format == 'Vinyl'
        },
        config: {
            title: 'Title'
        }
    }),

    chart => chart.animate({
        data: {
            filter: record =>
                record.Format == 'DVD' ||
                record.Format == 'Tapes' ||
                record.Format == 'Cassette' ||
                record.Format == 'Vinyl' ||
                record.Format == 'CD'
        },
        config: {
            title: 'Title'
        }
    }),

    chart => chart.animate({
        data: {
            filter: record =>
                record.Format == 'DVD' ||
                record.Format == 'Other' ||
                record.Format == 'Tapes' ||
                record.Format == 'Cassette' ||
                record.Format == 'Vinyl' ||
                record.Format == 'CD'
        },
        config: {
            title: 'Title'
        }
    }),

    chart => chart.animate({
        data: {
            filter: record =>
                record.Format == 'DVD' ||
                record.Format == 'Other' ||
                record.Format == 'Tapes' ||
                record.Format == 'Download' ||
                record.Format == 'Cassette' ||
                record.Format == 'Vinyl' ||
                record.Format == 'CD'
        },
        config: {
            title: 'Title'
        }
    }),
    chart => chart.animate({
        data: {
            filter: record =>
                record.Format == 'DVD' ||
                record.Format == 'Other' ||
                record.Format == 'Tapes' ||
                record.Format == 'Download' ||
                record.Format == 'Streaming' ||
                record.Format == 'Cassette' ||
                record.Format == 'Vinyl' ||
                record.Format == 'CD'
        },
        config: {
            title: 'Title'
        }
    }),
chart => chart.feature('tooltip',true)
];

export default testSteps;