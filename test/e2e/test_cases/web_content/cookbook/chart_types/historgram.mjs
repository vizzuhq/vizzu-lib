import { data } from '../../../../test_data/chart_types_eu.mjs'

class Binner {
	constructor(min, max, count) {
		this.min = min
		this.max = max
		this.count = count
	}

	getBin(x) {
		let bin = Math.floor(((x - this.min) / (this.max - this.min)) * this.count)
		if (bin < 0) bin = 0
		if (bin >= this.count) bin = this.count - 1
		return bin
	}

	getBinStr(x) {
		const bin = this.getBin(x)
		const binWidth = (this.max - this.min) / this.count
		const binStart = this.min + bin * binWidth
		const binEnd = binStart + binWidth
		return `${binStart.toFixed(2)} - ${binEnd.toFixed(2)}`
	}

	getBinSeries(measure, name = 'bin') {
		const bins = measure.values.map((v) => this.getBinStr(v))
		return { name, type: 'dimension', values: bins }
	}
}

const continous = 'Value 1 (+)'

const measure = data.series.find((s) => s.name === continous)

// sort the values
measure.values.sort()

/* const animOptions = {
  x: { delay: 0, duration: 0.3 },
  y: { delay: 0, duration: 0.3 },
  show: { delay: 0, duration: 0.3 },
  hide: { delay: 0, duration: 0.3 }
} */

const testSteps = [
	(chart) => {
		const binner = new Binner(0, 1, 12)
		data.series.push(binner.getBinSeries(measure))

		return chart.animate({
			data: Object.assign(
				{
					series: [binner.getBinSeries(measure)]
				},
				data
			),
			config: {
				channels: {
					x: 'bin',
					y: 'count()',
					label: 'count()',
					noop: null
				},
				title: 'Histogram of Value 1',
				geometry: 'rectangle'
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: 0
					}
				}
			}
		})
	}
]
/*
for (let i = 0; i < 10; i++) {
	let width = (100 - 9.5*i)/100;
	let binCount = Math.round(12 / width);	
	testSteps.push(chart => {
		let binner = new Binner(0, 1, binCount);
		return chart.animate({ data: { series: [ binner.getBinSeries(measure) ] }}, animOptions);
	});
}
*/
export default testSteps
