import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Bulgaria' ||
					record.Country === 'Cyprus' ||
					record.Country === 'Denmark' ||
					record.Country === 'Estonia' ||
					record.Country === 'Greece' ||
					record.Country === 'Spain' ||
					record.Country === 'Hungary'
			}),
			config: {
				channels: {
					x: 'Year',
					y: 'Country_code',
					color: { set: 'Value 5 (+/-)', range: { min: '-60', max: '73' } }
				},
				title: 'Heatmap with Color Gradient'
			},
			style: {
				plot: {
					marker: {
						colorGradient:
							'#3d51b8 0, #6389ec 0.15, #9fbffa 0.35, #d5d7d9 0.5, #f4b096 0.65, #e36c56 0.85, #ac1727 1'
					}
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps
