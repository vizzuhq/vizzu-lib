import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data: data_6,
				config: {
					x: 'Joy factors',
					y: 'Value 2 (+)',
					color: 'Joy factors',
					label: 'Value 2 (+)'
				}
			},
			0
		),

	(chart) => {
		let lastPointerX = null
		let progress = null

		const animation = chart.animate({
			x: 'Country',
			y: 'Value 2 (+)',
			color: 'Country',
			label: 'Value 2 (+)'
		})

		const seek = (percent) => {
			animation.activated.then((control) => {
				control.pause()
				control.seek(percent + '%')
			})
		}

		chart.on('update', (ev) => {
			progress = ev.detail.progress
		})

		chart.on('click', (event) => {
			event.preventDefault()
		})

		chart.on('pointerdown', (event) => {
			lastPointerX = event.detail.position.x
			event.preventDefault()
		})

		chart.on('pointerup', (event) => {
			lastPointerX = null
			event.preventDefault()
		})

		chart.on('pointermove', (event) => {
			if (lastPointerX) {
				const pointerX = event.detail.position.x
				const diff =
					(2 * (pointerX - lastPointerX)) / chart.feature.htmlCanvas.element.width
				seek((progress + diff) * 100)
				lastPointerX = pointerX
			}
			event.preventDefault()
		})

		return animation
	}
]

export default testSteps
