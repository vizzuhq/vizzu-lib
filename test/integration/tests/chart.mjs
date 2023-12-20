import DataCollection from './data.mjs'
import ConfigCollection from './config.mjs'

export default class Chart {
	static animate(params) {
		return (chart) => {
			const animate = {}

			let data = params.data
			if (data) {
				if (typeof data === 'string') {
					data = DataCollection[data]
				}
				animate.data = data
			}

			let config = params.config
			if (config) {
				if (typeof config === 'string') {
					config = ConfigCollection[config]
				}
				animate.config = config
			}

			const style = params.style
			if (style) {
				animate.style = style
			}

			return chart.animate(animate)
		}
	}
}
