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
					noop: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Distribution',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: ['Year', 'Value 3 (+)'],
						y: ['Country', 'Value 2 (+)'],
						color: 'Country'
					},
					title: '+ Continuous',
					geometry: 'rectangle',
					orientation: 'horizontal'
				}
			},
			{
				geometry: {
					delay: 0.7,
					duration: 1
				}
			}
			/*
     ,
    {
         geometry: {
            delay: 1.7,
            duration: 1
        },
         y: {
            delay: 1,
        },
        x: {
            delay: 0
        }
     }
     */
		)
]

export default testSteps
