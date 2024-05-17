import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Year'] },
					y: { attach: ['Value 5 (+/-)'] },
					size: { attach: ['Value 2 (+)'] }
				},
				coordSystem: 'polar'
			}
		})
]

const geometries = ['rectangle', 'circle', 'area', 'line']

for (let i = 0; i < geometries.length; i++) {
	testSteps.push((chart) => chart.animate({ geometry: geometries[i] }))
	for (let j = i + 1; j < geometries.length; j++) {
		testSteps.push((chart) => chart.animate({ geometry: geometries[j] }))
		testSteps.push((chart) => chart.animate({ geometry: geometries[i] }))
	}
}

export default testSteps
