import { data } from '../../../test_data/icicle.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors/Country', 'Value 2 (+)'],
					y: {
						set: ['Type'],
						title: 'Editable title',
						range: { min: '0%', max: '100%' }
					},
					label: 'Joy factors/Country',
					color: 'Joy factors/Country'
				},
				orientation: 'vertical',
				title: 'Icicle chart'
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: 0,
						label: {
							orientation: 'vertical',
							angle: 3.14 * -1
						}
					}
				}
			}
		})
]

export default testSteps
