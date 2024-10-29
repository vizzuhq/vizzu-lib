import Vizzu from '../dist/vizzu.min.js'

class MdChart {
	constructor(data, id) {
		this.data = data
		this.id = id
	}

	async create(snippets) {
		const animations = (await MdChart.loadAnimations(snippets)).map((anims) => ({ anims }))
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
					// remove if it can be found in the prevChart
					if (snippet.initDataFilter) {
						animTarget.data = { filter: snippet.initDataFilter }
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
						chart = snippet.anims[i](chart, {})
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

	static async loadAnimation(url, returnOriginal = false) {
		try {
			const response = await fetch(url)
			if (!response.ok) throw new Error(`Error fetching: ${response.statusText}`)
			const code = await response.text()
			return new Function( // eslint-disable-line no-new-func
				'chart',
				returnOriginal ? `${code}; return chart;` : `return ${code}`
			)
		} catch (error) {
			console.error('Error during animation load or execution:', error)
		}
	}

	static async loadAnimations(animations) {
		const steps = []
		for (const animation of animations) {
			if (typeof animation === 'string') {
				const func = await MdChart.loadAnimation(`./${animation}.js`)
				steps.push([(chart) => func(chart)])
			} else if (Array.isArray(animation)) {
				const animSteps = []
				for (const subAnimation of animation) {
					if (typeof subAnimation === 'string') {
						const func = await MdChart.loadAnimation(`./${subAnimation}.js`)
						animSteps.push((chart) => func(chart))
					} else if (typeof subAnimation === 'object' && subAnimation.name) {
						const { name, returnOriginal } = subAnimation
						const func = await MdChart.loadAnimation(`./${name}.js`, returnOriginal)
						animSteps.push((chart) => func(chart))
					}
				}
				steps.push(animSteps)
			} else if (typeof animation === 'object' && animation.name) {
				const { name, returnOriginal } = animation
				const func = await MdChart.loadAnimation(`./${name}.js`, returnOriginal)
				steps.push([(chart) => func(chart)])
			}
		}
		return steps
	}
}

export default MdChart
