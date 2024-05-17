import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		function urlToImage(url) {
			return new Promise((resolve) => {
				const image = new Image()
				image.addEventListener('load', () => {
					resolve(image)
				})
				image.src = url
			})
		}

		const bgImage = await urlToImage(
			'data:image/gif;base64,R0lGODlhAwACAPIAAJLf6q/i7M/r8un0+PT6+/n8/QAAAAAAACH5BAQAAAAALAAAAAADAAIAAAMEWBMkkAA7'
		)

		chart.on('background-draw', (event) => {
			event.renderingContext.drawImage(
				bgImage,
				0,
				0,
				event.detail.rect.size.x,
				event.detail.rect.size.y
			)
			event.preventDefault()
		})

		return chart
	},
	(chart) => {
		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: { set: 'Value 2 (+)', interlacing: false },
				title: 'Background image'
			}
		})
	}
]

export default testSteps
