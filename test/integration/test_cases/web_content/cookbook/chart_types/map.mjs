import { data } from '../../../../test_data/capitals.mjs'

const testSteps = [
	async (chart) => {
		function urlToImage(url) {
			return new Promise((resolve, reject) => {
				const image = new Image()
				image.addEventListener('load', () => {
					resolve(image)
				})
				image.addEventListener('error', () => {
					reject(new Error('Failed to load image: ' + url))
				})
				image.src = url
			})
		}

		const map = await urlToImage(
			'https://upload.wikimedia.org/wikipedia/commons/5/51/BlankMap-Equirectangular.svg'
		)

		chart.on('plot-area-draw', (event) => {
			const coordSystem = chart.feature.coordSystem
			const p0 = coordSystem.toCanvas({ x: 0, y: 0 })
			const p1 = coordSystem.toCanvas({ x: 1, y: 1 })
			event.renderingContext.globalAlpha = 0.25 // the map image is too dark
			event.renderingContext.drawImage(map, p0.x, p0.y, p1.x - p0.x, p1.y - p0.y)
			event.renderingContext.globalAlpha = 1
			event.preventDefault()
		})

		chart.feature('tooltip', true)

		return chart.animate({
			data,
			config: {
				title: 'Population of Capitals',
				x: {
					set: 'Longitude',
					range: { min: -180, max: +180 },
					axis: false,
					interlacing: false,
					markerGuides: false,
					title: null
				},
				y: {
					set: 'Latitude',
					range: { min: -90, max: +90 },
					axis: false,
					interlacing: false,
					markerGuides: false,
					title: null
				},
				noop: 'Capital',
				size: 'Population',
				geometry: 'circle'
			}
		})
	}
]

export default testSteps
