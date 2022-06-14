import { data } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => {
        chart.on('plot-axis-label-draw', event => {
            let year = parseFloat(event.data.text);
            if (!isNaN(year) && year % 5 != 0)
                event.preventDefault();
        });
            return chart.animate(
            {
            data: Object.assign(data, {
                filter: record =>
                    record.Format == 'DVD' ||
                    record.Format == 'Other' ||
                    record.Format == 'Tapes' ||
                    record.Format == 'Download' ||
                    record.Format == 'Streaming' ||
                    record.Format == 'Cassette' ||
                    record.Format == 'Vinyl' ||
                    record.Format == 'CD'
            }),
            config: {
                channels: {
                    x: { set: ['Format', 'Revenue [m$]'] },
                    y: { set: 'Year' },
                    color: { set: 'Format' }
                },
                title: 'Stream graph vertical',
                geometry: 'area',
                align: 'center'
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
        });
    },
    chart => chart.feature('tooltip',true)
];

export default testSteps;