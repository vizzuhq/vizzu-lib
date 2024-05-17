import { data } from '../../test_data/data_missing_long_more_conti.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: {
						attach: ['Categ. Parent', 'Values 1'],
						range: { min: '110%', max: '0%' }
					},
					x: { attach: ['Timeseries'] },
					color: { attach: ['Categ. Parent'] },
					label: { attach: ['Values 1'] }
				},
				title: 'Column More Conti. elements'
			}
		})
]

export default testSteps
