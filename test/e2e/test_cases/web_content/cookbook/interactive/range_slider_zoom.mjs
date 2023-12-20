import { data } from '../../../../test_data/chart_types_eu.mjs'
import { LastAnimation } from '../../../../utils/lastanimation.mjs'

const testSteps = [
	(chart) => {
		chart.feature(new LastAnimation())

		const rangeMax = 21
		const rangeMin = 2
		let range = rangeMax
		let finished = true

		const range2Value = (x) => (1 - (x - rangeMin) / (rangeMax - rangeMin)) * 100
		const value2Range = (v) => rangeMin + (1 - v / 100) * (rangeMax - rangeMin)

		const input = document.createElement('input')
		input.id = 'slider'
		input.type = 'range'
		input.value = range2Value(range)
		input.style = 'width: 200px; position: absolute; top: 540px; left: 30px;'
		chart.feature.htmlCanvas.element.parentElement.appendChild(input)

		function update() {
			if (!finished) return
			finished = false
			const rangeTarget = range
			chart
				.animate(
					{
						x: {
							range: {
								min: rangeMax - rangeTarget + 0.5,
								max: rangeMax - 0.5
							}
						}
					},
					{ duration: '50ms', easing: 'linear' }
				)
				.then((chart) => {
					finished = true
					if (rangeTarget !== range) update()
					return chart
				})
		}

		input.oninput = (e) => {
			range = value2Range(e.target.value)
			update()
		}

		return chart.animate(
			{
				data,
				config: {
					x: {
						set: 'Year',
						range: {
							min: 0.5,
							max: rangeMax - 0.5
						}
					},
					y: 'Value 5 (+/-)',
					title: 'Slider Zoom',
					geometry: 'line'
				}
			},
			0
		)
	},
	(chart) => {
		const input = document.getElementById('slider')
		input.value = '20'
		const event = new Event('input')
		input.dispatchEvent(event)
		return chart.feature.lastAnimation.last()
	}
]

export default testSteps
