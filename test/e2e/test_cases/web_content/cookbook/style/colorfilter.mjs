import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js')
		return chart
	},

	(chart) =>
		chart.animate({
			data,
			config: {
				x: 'Year',
				y: ['Value 2 (+)', 'Joy factors'],
				color: 'Joy factors',
				title: 'custom colorfilter'
			}
		}),

	(chart) => {
		function filteredStyle(style, colorfilter, level = 0) {
			for (const param in style) {
				if (typeof style[param] === 'object') {
					style[param] = filteredStyle(style[param], colorfilter, level + 1)
				} else {
					const isColor = param.endsWith('color') || param.endsWith('Color')
					const ignore = level === 0 && param === 'backgroundColor'
					if (isColor && !ignore && level > 0) {
						style[param] = colorfilter(style[param])
					}
				}
			}
			if (level === 0) {
				style.plot.marker.colorPalette = style.plot.marker.colorPalette
					.split(' ')
					.map(colorfilter)
					.join(' ')
				const re = /\s*,\s*/
				style.plot.marker.colorGradient = style.plot.marker.colorGradient
					.split(re)
					.map((step) => {
						const parts = step.split(' ')
						parts[0] = colorfilter(parts[0])
						return parts.join(' ')
					})
					.join(',')
			}
			return style
		}

		return chart.animate({
			style: filteredStyle({ ...chart.getComputedStyle() }, (color) => {
				const gray = tinycolor(color).desaturate(100)
				return gray.setAlpha(gray.getAlpha() * 0.3).toHex8String()
			})
		})
	}
]

export default testSteps
