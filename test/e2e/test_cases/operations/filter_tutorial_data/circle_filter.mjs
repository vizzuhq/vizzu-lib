import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Values 1'] },
					y: { attach: ['Values 2'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Timeseries'] },
					size: { attach: ['Values 3'] },
					noop: { attach: ['Categ. Child'] }
				},
				title: 'Operations: Scatter Plot.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Timeseries !== '2019' && record.Timeseries !== '2021'
			},
			config: {
				title: 'Operations:  Scatter Plot - Filtered.'
			}
		})
]

export default testSteps
