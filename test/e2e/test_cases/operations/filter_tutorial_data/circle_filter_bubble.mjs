import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					label: { attach: ['Values 1'] },
					color: { attach: ['Timeseries'] },
					size: { attach: ['Values 1', 'Categ. Child'] }
				},
				title: 'Operation: Bubble Chart.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Timeseries !== '2019' && record.Timeseries !== '2020'
			},
			config: {
				title: 'Operation: Bubble - Filtered.'
			}
		})
]

export default testSteps
