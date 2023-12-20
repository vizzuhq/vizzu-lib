import { data } from '../../../../../test_data/chart_types_eu.mjs'

/* eslint-disable no-template-curly-in-string */

const testSteps = [
	(chart) =>
		chart.animate(
			{
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
						y: { set: ['Country', 'Value 2 (+)'] }
					},
					title: 'Value',
					geometry: 'area',
					legend: null,
					orientation: 'horizontal',
					align: 'stretch',
					split: false
				},
				style: {
					plot: {
						marker: { rectangleSpacing: 0 }
					}
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 1
				},
				x: {
					delay: 0,
					duration: 0
				},
				y: {
					delay: 0,
					duration: 1
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Stacked Area Chart',
					geometry: 'area',
					orientation: 'horizontal',
					align: 'min',
					legend: null
				},
				style: {
					plot: {
						marker: { rectangleSpacing: null }
					}
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 1
				},
				x: {
					delay: 0,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				},
				y: {
					delay: 0,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Overlay Area Chart',
					geometry: 'area',
					orientation: 'horizontal',
					align: 'min',
					legend: null
				},
				style: {
					plot: {
						marker: {
							fillOpacity: 0.5,
							borderWidth: 0
						}
					}
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 1
				},
				x: {
					delay: 0,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				},
				y: {
					delay: 0.5,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Splitted Area Chart',
					geometry: 'area',
					orientation: 'horizontal',
					align: 'min',
					split: true,
					legend: null
				},
				style: {
					plot: {
						marker: {
							rectangleSpacing: null,
							fillOpacity: null,
							borderWidth: null
						}
					}
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0.5,
					duration: 1
				},
				x: {
					delay: 0,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				},
				y: {
					delay: 0,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				}
			}
		),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Value 2 (+)'] },
						color: { set: 'Country' }
					},
					title: 'Line Chart',
					geometry: 'line',
					orientation: 'horizontal',
					align: 'min',
					split: false,
					legend: null
				}
			},
			{
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 1
				},
				x: {
					delay: 0,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				},
				y: {
					delay: 0.2,
					duration: 1,
					easing: 'cubic-bezier(${65},${0},${65},${1})'
				}
			}
		)
]

export default testSteps
