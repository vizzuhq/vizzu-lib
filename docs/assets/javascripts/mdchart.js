import Vizzu from '../dist/vizzu.min.js'

class MdChart {
	constructor(data, id) {
		this.data = data
		this.id = id
	}

	async create(snippets) {
		const animations = await MdChart.loadAnimations(snippets)
		let chart = Promise.resolve()
		for (let i = 0; i < animations.length; i++) {
			const number = i + 1
			chart = this.animate(('0' + number).slice(-2), animations[i], chart)
		}
	}

	restore(number, snippet, prevChart, snapshot, chart) {
		const div = document.getElementById(this.id + '_' + number)

		return Promise.all([chart, prevChart]).then((results) => {
			let chart = results[0]
			const prevChart = results[1]
			div.classList.remove('loading')
			div.classList.add('playing')
			let animTarget
			if (snapshot && !prevChart) {
				animTarget = snapshot
			} else {
				animTarget = {}
				if (prevChart) {
					animTarget.config = Object.assign({}, prevChart.config)
					animTarget.style = Object.assign({}, prevChart.style)
					if (snippet?.assets?.initDataFilter) {
						animTarget.data = { filter: snippet.assets.initDataFilter }
					}
				}
			}
			chart = chart.animate(animTarget)
			chart.activated.then((control) => control.seek('100%'))
			return chart
		})
	}

	animate(number, snippet, prevChart) {
		const div = document.getElementById(this.id + '_' + number)
		div.classList.add('loading')

		let snapshot

		let chart = new Vizzu(div, { data: this.data }).initializing

		chart = this.restore(number, snippet, prevChart, snapshot, chart)

		chart = chart.then((chart) => {
			snapshot = chart.store()
			return chart
		})

		let firstRun = true
		let clicked = false
		div.onclick = () => {
			if (!clicked) {
				clicked = true

				chart = this.restore(number, snippet, prevChart, snapshot, chart)
				chart.then(() => {
					div.classList.remove('replay')
					div.classList.add('playing')
				})
				for (let i = 0; i < snippet.anims.length; i++) {
					chart = chart.then((chart) => {
						chart = snippet.anims[i](chart, this.data, snippet?.assets)
						if (this.id === 'tutorial' && firstRun && chart.activated) {
							chart.activated.then((control) => control.seek('100%'))
						}
						return chart
					})
				}
				chart.then(() => {
					div.classList.remove('playing')
					div.classList.add('replay')
					clicked = false
					firstRun = false
				})

				return chart
			}
		}
		div.click()

		return chart
	}

	static async loadAnimation(url, config) {
		try {
			let code
			if (
				typeof window === 'undefined' &&
				typeof process !== 'undefined' &&
				process.versions?.node
			) {
				const fs = await import('fs').then((module) => module.promises)
				code = await fs.readFile(
					config?.nodeBaseUrl ? `${config.nodeBaseUrl}/${url}` : url,
					'utf8'
				)
			} else {
				const response = await fetch(
					config?.browserBaseUrl ? `${config.browserBaseUrl}/${url}` : url
				)
				if (!response.ok) throw new Error(`Error fetching: ${response.statusText}`)
				code = await response.text()
			}
			const replace = config?.replace
			if (Array.isArray(replace)) {
				replace.forEach(([searchValue, replaceValue]) => {
					code = code.replaceAll(searchValue, replaceValue)
				})
			}
			const returnOriginal = config?.returnOriginal
			return new Function( // eslint-disable-line no-new-func
				'chart',
				'data',
				'assets',
				returnOriginal ? `${code}; return chart;` : `return ${code}`
			)
		} catch (error) {
			console.error('Error during animation load or execution:', error)
		}
	}

	static async loadAnimations(animations, nodeBaseUrl = undefined, browserBaseUrl = undefined) {
		const steps = []
		const baseUrl = {
			nodeBaseUrl,
			browserBaseUrl
		}

		async function loadAnimation(animation) {
			let anim
			if (typeof animation === 'string') {
				anim = await MdChart.loadAnimation(`${animation}.js`, baseUrl)
			} else if (typeof animation === 'object' && animation.name) {
				const { name, ...config } = animation
				anim = await MdChart.loadAnimation(`${name}.js`, Object.assign({}, config, baseUrl))
			}
			return (chart, data, assets) => anim(chart, data, assets)
		}

		for (const animation of animations) {
			const step = { anims: [] }

			let subAnimations
			if (Array.isArray(animation)) {
				subAnimations = animation
			} else {
				subAnimations = animation?.anims ?? []
				if (animation?.assets) step.assets = animation.assets
			}

			for (const subAnimation of subAnimations) {
				const anim = await loadAnimation(subAnimation)
				step.anims.push(anim)
			}
			steps.push(step)
		}
		return steps
	}
}

export default MdChart
