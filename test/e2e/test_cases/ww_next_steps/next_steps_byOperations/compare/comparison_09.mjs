import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 2 (+)', 'Country'], split: true },
					color: 'Country'
				},
				title: 'Trellis Area',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'Value 2 (+)', split: false }
				},
				title: 'CHG Geom., Split off & Unstack to Compare Components',
				geometry: 'line'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
