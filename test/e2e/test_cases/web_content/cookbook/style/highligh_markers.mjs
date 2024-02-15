import { data } from '../../../../test_data/chart_types_eu.mjs'

let highlighter

class Highlighter {
	constructor(chart, animOptions) {
		this.chart = chart
		this.animOptions = animOptions
		this.metadata = this.chart.data
		this.defaultPalette = this.chart.getComputedStyle().plot.marker.colorPalette.split(' ')
		this.palette = this.defaultPalette

		const colorchannels = this.chart.config.channels.color.set
		if (colorchannels.length === 0) this.colorchannel = null
		else if (colorchannels.length === 1) this.colorchannel = colorchannels[0].name
		else throw new Error('charts with multiple color series are not supported by highlighter')
	}

	highlight(dataseries, categories) {
		if (this.colorchannel && dataseries !== this.colorchannel) {
			throw new Error('Color channel must be empty or match the data series to be selected')
		}

		const metadata = this._getSeriesMetadata(dataseries)
		if (!metadata) return this.chart
		const allCategories = metadata.categories

		this.palette = []
		for (let i = 0; i < allCategories.length; i++) {
			const category = allCategories[i]
			this.palette.push(
				categories.includes(category)
					? this.defaultPalette[i % this.defaultPalette.length]
					: '#dfdfdf'
			)
		}

		return this.chart.animate(
			{
				config: { channels: { color: { set: dataseries } } },
				style: { plot: { marker: { colorPalette: this.palette.join(' ') } } }
			},
			this.animOptions
		)
	}

	clear() {
		this.palette = this.defaultPalette
		return this.chart.animate(
			{
				config: { channels: { color: { set: this.colorchannel } } },
				style: { plot: { marker: { colorPalette: this.palette.join(' ') } } }
			},
			this.animOptions
		)
	}

	_compareArrays(a, b) {
		return a.length === b.length && a.every((element, index) => element === b[index])
	}

	_getSeriesMetadata(dataseries) {
		for (const metadata of this.metadata.series)
			if (metadata.name === dataseries) return metadata
		return null
	}
}

const testSteps = [
	(chart) => {
		return chart.animate({
			data,
			config: {
				x: 'Year',
				color: 'Year',
				y: 'Value 2 (+)',
				title: 'Highlight markers'
			}
		})
	},
	(chart) => {
		highlighter = new Highlighter(chart, 0.5)
		return highlighter.highlight('Year', ['10', '14'])
	},
	(chart) => {
		return highlighter.highlight('Year', ['02', '06', '16'])
	},
	(chart) => {
		return highlighter.clear()
	}
]

export default testSteps
