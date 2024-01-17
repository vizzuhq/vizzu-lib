import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		const baseUrl = 'https://cdn.jsdelivr.net/npm/gif.js@0.2.0/dist'

		await import(baseUrl + '/gif.min.js')

		const workerBlob = await fetch(baseUrl + '/gif.worker.js').then((response) => {
			if (!response.ok) {
				throw new Error('Network response was not OK')
			}
			return response.blob()
		})

		const gif = new GIF({
			workers: 4,
			workerScript: URL.createObjectURL(workerBlob),
			quality: 10,
			width: chart.feature.htmlCanvas.element.width,
			height: chart.feature.htmlCanvas.element.height
		})

		chart.on('draw-complete', (event) => {
			gif.addFrame(event.renderingContext, { copy: true, delay: 25 })
		})

		gif.on('finished', function (blob) {
			window.open(URL.createObjectURL(blob))
		})

		return chart
			.animate({
				data,
				config: {
					x: 'Year',
					y: ['Value 2 (+)', 'Joy factors'],
					color: 'Joy factors',
					title: 'Anim GIF Export'
				}
			})
			.then((chart) => {
				gif.render()
				return chart
			})
	}
]

export default testSteps
