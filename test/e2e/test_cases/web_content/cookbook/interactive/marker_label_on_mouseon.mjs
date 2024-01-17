import { data } from '../../../../test_data/chart_types_eu.mjs'
import { Mouse } from '../../../../utils/mouse.mjs'

const testSteps = [
	(chart) => {
		const labelToShow = { value: '' }

		chart.on('pointermove', (event) => {
			if (event.target?.tagName === 'plot-marker') {
				labelToShow.value = event.target.values['Value 5 (+/-)']
				chart.feature.rendering.update()
			} else {
				labelToShow.value = ''
				chart.feature.rendering.update()
			}
		})

		chart.on('plot-marker-label-draw', (event) => {
			if (event.target.parent.values['Value 5 (+/-)'] !== labelToShow.value)
				event.preventDefault()
		})

		return chart.animate({
			data,
			config: {
				x: 'Year',
				y: 'Value 5 (+/-)',
				label: 'Value 5 (+/-)',
				title: 'Label shown on mouse-on'
			}
		})
	},

	(chart) => {
		new Mouse(chart).move(477, 178)
		return chart.animate({}, 1)
	}
]

export default testSteps
