import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)',
				title: 'Motion blur'
			}
		})
	},

	(chart) => {
		const images = []

		function drawImages(ctx) {
			ctx.drawImage(images[0], 0, 0)
			ctx.globalAlpha = 0.5
			images.map((image) => ctx.drawImage(image, 0, 0))
			return images.length > 0
		}

		function drawComplete(event) {
			const ctx = event.renderingContext
			const canvas = chart.feature.htmlCanvas.element
			const imageData = ctx.getImageData(0, 0, canvas.clientWidth, canvas.clientHeight)
			createImageBitmap(imageData).then((image) => {
				images.push(image)
			})
			if (images.length > 10) images.shift()
			drawImages(ctx)
		}

		chart.on('draw-complete', drawComplete)

		return chart.animate({
			coordSystem: 'polar',
			y: 'Joy factors',
			x: 'Value 2 (+)'
		})
	}
]

export default testSteps
