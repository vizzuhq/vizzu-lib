import { data } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => chart.animate(
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
            config: chart.constructor.presets.violin({
                x: 'Year',
                y: 'Revenue [m$]',
                splittedBy: 'Format',
                title: 'Violin Graph'
            }),
            style: {
                plot: {
                    yAxis: {
                        interlacing: { color: '#ffffff00' }
                    },
                    xAxis: {
                        label: {
                            angle: '-45deg'
                        }
                    }
                }
            }
        })
];

export default testSteps;