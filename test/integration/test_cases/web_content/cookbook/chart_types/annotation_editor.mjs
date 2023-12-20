import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://cdn.jsdelivr.net/npm/markerjs2@2.29.0/markerjs2.min.js')

		const markerArea = new markerjs2.MarkerArea(chart.feature.htmlCanvas.element)

		markerArea.show()

		markerArea.addEventListener('render', (event) => {
			navigator.clipboard.writeText(JSON.stringify(event.state))
			alert('Annotation JSON object copied to clipboard!')
		})

		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)',
				title: 'Chart with Annotations'
			}
		})
	}
]

export default testSteps
