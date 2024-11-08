import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: { attach: ['Joy factors'] },
					size: { attach: ['Value 2 (+)'] },
					label: { attach: ['Joy factors'] }
				},
				title: '1D, 1C - Treemap'
			},
			style: {
				plot: {
					marker: {
						label: { fontSize: 14 }
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Joy factors'] },
					x: { attach: ['Value 2 (+)'], range: { min: '0%', max: '133%' } },
					size: { detach: ['Value 2 (+)'] },
					label: { detach: ['Joy factors'] }
				},
				title: 'X C, Y D (Y first) - Radial',
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Joy factors'] },
					x: { detach: ['Value 2 (+)'] },
					size: { attach: ['Value 2 (+)'] },
					label: { attach: ['Joy factors'] }
				},
				title: 'X C, Y D (X first) - Treemap',
				coordSystem: 'cartesian'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					lightness: { attach: ['Value 2 (+)'] },
					label: { attach: ['Country_code'], detach: 'Joy factors' },
					size: { attach: ['Country_code'] }
				},
				title: 'X C+D, Y D - Treemap.'
			},
			style: {
				plot: {
					marker: {
						label: { fontSize: 8 }
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Joy factors'] },
					x: {
						attach: ['Value 2 (+)', 'Country_code'],
						range: { min: '0%', max: '133%' }
					},
					size: { detach: ['Value 2 (+)'] }
				},
				title: 'X C+D, Y D (Y first) - Radial',
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Joy factors'] },
					x: { detach: ['Value 2 (+)', 'Country_code'] },
					size: { attach: ['Value 2 (+)'] }
				},
				title: 'X C+D, Y D (X first) - Treemap',
				coordSystem: 'cartesian'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Joy factors'] },
					x: {
						attach: ['Value 2 (+)', 'Country_code'],
						range: { min: '0%', max: '133%' }
					},
					size: { detach: ['Value 2 (+)'] }
				},
				title: 'X C+D, Y D (Y first) - Radial',
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				orientation: 'vertical',
				channels: {
					y: { attach: ['Value 1 (+)'] }
				},
				title: 'X C+D, Y D+C - Radial-Mekko'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Joy factors', 'Value 1 (+)'] },
					x: { detach: ['Value 2 (+)', 'Country_code'] },
					size: { attach: ['Value 2 (+)'] }
				},
				title: 'X C+D, Y D+C (X first) - Treemap',
				coordSystem: 'cartesian'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					label: { detach: ['Country_code'], attach: ['Joy factors'] },
					size: { detach: ['Country_code'] },
					lightness: { detach: ['Value 2 (+)'] }
				},
				title: 'X C+D, Y C - Treemap'
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: 14,
							position: 'center'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Joy factors', 'Value 1 (+)'] },
					x: { attach: ['Value 2 (+)'], range: { min: '0%', max: '133%' } },
					size: { detach: ['Value 2 (+)'] },
					label: { detach: ['Joy factors'] }
				},
				title: 'X D+C, Y C (Y first) - Radial-Mekko',
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Value 1 (+)'] }
				},
				title: 'X C, Y C (Y first) - Radial'
			}
		})
]

export default testSteps
