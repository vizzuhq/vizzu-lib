import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)'
			}
		}),

	(chart) => {
		const input = document.createElement('input')
		input.type = 'range'
		input.min = 0
		input.max = 1000
		input.style = 'width: 500px; position: absolute; top: 540px; left: 30px;'
		chart.feature.htmlCanvas.element.parentElement.appendChild(input)

		const animation = chart.animate({
			x: 'Country',
			y: 'Value 2 (+)',
			color: 'Country',
			label: 'Value 2 (+)'
		})

		input.oninput = (e) => {
			animation.activated.then((control) => {
				control.pause()
				control.seek((100 * e.target.value) / 1000 + '%')
			})
		}

		chart.on('update', (ev) => {
			input.value = ev.detail.progress * 1000
		})

		return animation
	}
]

export default testSteps
