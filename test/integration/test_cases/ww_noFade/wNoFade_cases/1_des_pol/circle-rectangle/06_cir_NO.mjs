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
					y: { set: 'Value 6 (+/-)' },
					x: { set: 'Value 2 (+)' },
					color: { set: 'Country' },
					noop: { set: 'Year' }
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: null,
					size: { set: 'Year' }
				},
				title: 'Stack Discrete'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						noop: null,
						size: { set: ['Joy factors'] }
					},
					title: 'Change Discrete'
				}
			},
			{
				duration: 0
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'Value 3 (+)' },
					x: { set: 'Value 5 (+/-)' }
				},
				title: 'Change Continuous'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						y: { set: ['Country', 'Value 3 (+)'] },
						x: { set: ['Joy factors', 'Value 5 (+/-)'] },
						size: null
					},
					title: 'Group new Discrete',
					geometry: 'rectangle'
				}
			},
			{
				geometry: {
					delay: 0.7,
					duration: 1
				}
			}
		)
]

export default testSteps
