import { data } from '../../../../test_data/chart_types_eu.mjs'
data.filter = (record) =>
	record.Country !== 'Germany' &&
	record.Country !== 'Italy' &&
	record.Country !== 'Hungary' &&
	record.Country !== 'Netherlands' &&
	record.Country !== 'Estonia' &&
	record.Country !== 'Belgium' &&
	record.Country !== 'Sweden' &&
	record.Country !== 'Poland' &&
	record.Country !== 'Malta' &&
	record.Country !== 'Romania' &&
	record.Country !== 'Greece' &&
	record.Country !== 'Slovakia' &&
	record.Country !== 'Ireland' &&
	record.Country !== 'Lithuania' &&
	record.Country !== 'Croatia' &&
	record.Country !== 'Slovenia' &&
	record.Country !== 'Portugal' &&
	record.Country !== 'Finland' &&
	record.Country !== 'United Kingdom'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Country'] },
					color: { attach: ['Country'] },
					label: { attach: ['Value 5 (+/-)'] }
				},
				title: 'Rectangle Marker Label PaddingBottom: Default'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label PaddingBottom: 40'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							paddingBottom: 40
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label PaddingBottom: null'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							paddingBottom: null
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label PaddingRight: 80'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							paddingBottom: null,
							paddingRight: 80
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label PaddingRight: -80'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							paddingRight: -80
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Value 2 (+)'], range: { min: 'auto', max: 'auto' } },
					x: { attach: ['Value 2 (+)'] },
					label: { detach: ['Value 5 (+/-)'], attach: ['Value 2 (+)', 'Country'] }
				},
				title: '1 Rectangle Marker Label PaddingBottom: 40',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							paddingRight: null,
							paddingBottom: 40
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: '2 Rectangle Marker Label PaddingRight: 80'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							paddingBottom: null,
							paddingRight: 80
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label PaddingRight: -80'
			},
			style: {
				plot: {
					marker: {
						label: {
							paddingRight: -80
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Rectangle Marker Label Padding: Default'
			},
			style: {
				plot: {
					marker: {
						label: {
							paddingRight: null,
							paddingBottom: null
						}
					}
				}
			}
		})
]

export default testSteps
