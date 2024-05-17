import { data } from '../../../../test_data/chart_types_eu.mjs'
import { Mouse } from '../../../../utils/mouse.mjs'

const testSteps = [
	(chart) => {
		const clickHandler = (event) => {
			const filter =
				event.target && event.target.tagName === 'plot-marker'
					? (record) => record.Year === event.target.categories.Year
					: null

			chart.animate({ data: { filter } })

			event.preventDefault()
		}

		chart.on('click', clickHandler)

		return chart.animate({
			data,
			config: {
				x: 'Year',
				y: 'Value 2 (+)',
				label: 'Value 2 (+)',
				title: 'Filter on click'
			}
		})
	},
	(chart) => {
		return new Mouse(chart).click(142, 168).expectedAnimation()
	},
	(chart) => {
		return new Mouse(chart).click(100, 100).expectedAnimation()
	}
]

export default testSteps
