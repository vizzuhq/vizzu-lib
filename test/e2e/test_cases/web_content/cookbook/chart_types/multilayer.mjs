import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		const container = chart.feature.htmlCanvas.element
		const container2 = document.createElement('canvas')

		container2.style = `
			width: ${container.width}px; 
			height: ${container.height}px; 
			position: absolute; 
			top: ${container.offsetTop}px; 
			left: ${container.offsetLeft}px;
		`
		container.parentElement.appendChild(container2)

		const chart2 = await new chart.constructor(container2).initializing

		return Promise.all([
			chart2.animate({
				data,
				config: {
					x: {
						set: 'Joy factors',
						markerGuides: true,
						labels: false,
						axis: false
					},
					y: { set: 'Value 1 (+)', interlacing: false, title: null },
					label: 'Value 1 (+)',
					geometry: 'line',
					title: ' '
				},
				style: {
					backgroundColor: '#ffffff00',
					plot: {
						marker: {
							colorPalette: '#FFA000'
						}
					}
				}
			}),
			chart.animate({
				data,
				config: {
					x: 'Joy factors',
					y: 'Value 2 (+)',
					title: 'Multi layer chart'
				}
			})
		])
	}
]

export default testSteps
