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
                config: chart.constructor.presets.violin({
                    x: 'Year',
                    y: 'Revenue [m$]',
                    splittedBy: 'Format',
                    title: 'Violin Graph'
                }),
                style: {
                    plot: {
                        paddingLeft: '1.2em',
                        yAxis: {
                            label: {
                                paddingRight: '0.8em'
                            },
                            interlacing: { color: '#ffffff00' }
                        },
                        xAxis: {
                            label: {
                                paddingTop: '0.8em'
                            }
                        }
                    }
                }
            });
    }
];

export default testSteps;