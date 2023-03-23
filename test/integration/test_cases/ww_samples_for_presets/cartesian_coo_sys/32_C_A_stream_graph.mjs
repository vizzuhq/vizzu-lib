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
            data: data,
            config: {
                channels: {
                    x: 'Year',
                    y: ['Format', 'Revenue [$]'],
                    color: 'Format'
                },
                title: 'Stream Graph',
                geometry: 'area',
                align: 'center'
            },
            style: {
                plot: {
                    yAxis: { interlacing: { color: '#ffffff00' },
                    yAxis:{ label:{ numberScale: 'K, M, B, T' } }
                    }
                }
            }
        });
    },
    chart => chart.feature('tooltip',true)
    ];

export default testSteps;