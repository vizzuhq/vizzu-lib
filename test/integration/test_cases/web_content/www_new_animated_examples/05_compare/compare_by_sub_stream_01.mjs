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
                record['Year'] > 1972
        }),
        config: {
            channels: {
                x: 'Year',
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
        config: {
            title: 'Title',
            split: true,
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: ['Revenue [$]', 'Year'],
                x: ['Format'],
            },
            title: 'Title',
            geometry: 'rectangle',
            align: 'min',
            split: false,
            sort: 'byValue',
            reverse: true
        }
    }),
    chart => chart.feature('tooltip', true)
];

export default testSteps;