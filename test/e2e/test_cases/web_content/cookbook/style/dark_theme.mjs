import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		const style = {
			backgroundColor: '#121212',
			fontFamily: 'Roboto, sans-serif',
			fontSize: '10px',

			legend: {
				label: {
					color: '#b9b9b9',
					fontSize: '1em',
					textAlign: 'left'
				},
				title: {
					color: '#b9b9b9',
					fontSize: '1.166667em',
					textAlign: 'left'
				}
			},

			plot: {
				marker: {
					colorGradient: '#003f5c 0, #58508d 0.25, #bc5090 0.5, #ff6361 0.75, #ffa600 1',
					colorPalette:
						'#003f5c #2f4b7c #665191 #a05195 #d45087 #f95d6a #ff7c43 #ffa600 #bea500 #819d10 #488f31 #00795e'
				},
				yAxis: {
					color: '#CCCCCCFF',
					label: {
						color: '#999999FF',
						fontSize: '1em'
					},
					title: {
						color: '#999999FF',
						fontSize: '1.166667em'
					},
					interlacing: {
						color: '#212121'
					}
				},
				xAxis: {
					label: {
						color: '#999999FF',
						fontSize: '1em'
					},
					title: {
						color: '#999999FF',
						fontSize: '1.166667em'
					},
					interlacing: {
						color: '#212121'
					}
				}
			},

			title: {
				color: '#f7f7f7',
				fontSize: '2.166667em',
				textAlign: 'center'
			},

			tooltip: {
				arrowSize: '8',
				color: '#212121',
				backgroundColor: '#003f5cFF',
				borderColor: '#D8D8D8FF',
				fontSize: '12px'
			},

			logo: {
				filter: 'color(#D8D8D8FF)'
			}
		}

		return chart.animate({
			data,
			config: {
				x: 'Year',
				y: ['Value 2 (+)', 'Joy factors'],
				color: 'Joy factors',
				title: 'Dark theme (color palette)'
			},
			style
		})
	},
	(chart) =>
		chart.animate({
			x: 'Year',
			y: 'Country_code',
			color: 'Value 3 (+)',
			title: 'Dark theme (color gradient)'
		})
]

export default testSteps
