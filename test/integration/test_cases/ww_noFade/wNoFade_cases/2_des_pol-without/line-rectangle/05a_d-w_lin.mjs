import { data } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Bulgaria' ||
					record.Country === 'Cyprus' ||
					record.Country === 'Czechia' ||
					record.Country === 'Denmark' ||
					record.Country === 'Estonia' ||
					record.Country === 'Greece' ||
					record.Country === 'Germany' ||
					record.Country === 'Spain' ||
					record.Country === 'Finland' ||
					record.Country === 'France' ||
					record.Country === 'Croatia' ||
					record.Country === 'Hungary'
			}),
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' },
					size: { set: 'Value 3 (+)' }
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Joy factors' },
					color: { set: 'Joy factors' },
					size: { set: 'Value 3 (+)' }
				},
				title: 'Stack Disc & Remove Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Country_code' },
						y: { set: 'Joy factors' },
						color: { set: 'Joy factors' },
						size: { set: 'Value 3 (+)' }
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
					x: { set: 'Country_code' },
					y: { set: 'Value 1 (+)' },
					color: { set: 'Joy factors' },
					size: { set: 'Value 3 (+)' }
				},
				title: 'Group new Disc & Add new Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: null,
						color: { set: 'Joy factors' },
						size: { set: ['Country_code', 'Value 1 (+)'] },
						lightness: { set: 'Value 3 (+)' }
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
