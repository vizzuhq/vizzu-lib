import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Joy factors'
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: 'Joy factors',
					color: 'Joy factors'
				},
				title: 'Stack Disc & Remove Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: 'Country_code',
						y: 'Joy factors',
						color: 'Joy factors'
					},
					title: 'Change Disc'
				}
			},
			{
				duration: 0.5
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Country_code',
					y: 'Value 2 (+)',
					color: 'Joy factors'
				},
				title: 'Group new Disc & Add previous Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: null,
						color: 'Joy factors',
						size: ['Country_code', 'Value 2 (+)']
					},
					title: 'Stack new Disc & Change Geoms & CoordSys',
					geometry: 'rectangle'
				}
			},
			{
				geometry: {
					delay: 0.5,
					duration: 1.5
				},
				x: {
					delay: 1
				}
			}
		)
]

export default testSteps
