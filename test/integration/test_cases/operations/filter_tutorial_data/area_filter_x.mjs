import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Timeseries',
					y: ['Values 1', 'Categ. Parent'],
					color: 'Categ. Parent',
					label: 'Values 1'
				},
				title: 'Operations: Area',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Timeseries !== '2019' && record.Timeseries !== '2020'
			},
			config: {
				title: 'Operations: Area - Filtered.'
			}
		})
]

export default testSteps
