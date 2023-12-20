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
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: 'Stacked Column Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Country' },
					color: null
				},
				title: 'Remove Color & Remove Conti & Change Orientation & recSpacing',
				orientation: 'vertical'
			},
			style: {
				plot: {
					marker: { rectangleSpacing: '0' }
				}
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Joy factors' },
						y: null,
						color: null
					},
					title: 'Change Discs & Change Orientation',
					orientation: 'horizontal'
				}
			},
			{
				duration: 0.5
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Joy factors' },
						y: { set: 'Value 2 (+)' },
						color: { set: 'Joy factors' }
					},
					title: 'Add new Disc Color & Add previous Conti & recSpacing',
					orientation: 'horizontal'
				},
				style: {
					plot: {
						marker: { rectangleSpacing: null }
					}
				}
			},
			{
				easing: 'cubic-bezier(.39,0,.35,.99)',
				y: {
					delay: 0,
					duration: 1
				}
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: { set: 'Joy factors' },
					size: { set: 'Value 2 (+)' }
				},
				title: 'Change CoordSys'
			}
		})
]

export default testSteps
