import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Categ. Parent'] },
					y: { attach: ['Values 1', 'Timeseries'] },
					color: { attach: ['Timeseries'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Area - Filter.'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Timeseries !== '2018' && record.Timeseries !== '2021'
			},
			config: {
				title: 'Operations: Area - Filtered.'
			}
		})
]

export default testSteps
