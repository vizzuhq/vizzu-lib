export async function loadAnimation(url, config) {
	try {
		let code
		if (typeof window !== 'undefined') {
			const response = await fetch(
				config?.browserBaseUrl ? `${config.browserBaseUrl}/${url}` : url
			)
			if (!response.ok) throw new Error(`Error fetching: ${response.statusText}`)
			code = await response.text()
		} else {
			const fs = await import('fs').then((module) => module.promises)
			code = await fs.readFile(
				config?.nodeBaseUrl ? `${config.nodeBaseUrl}/${url}` : url,
				'utf8'
			)
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

export async function loadAnimations(
	animations,
	nodeBaseUrl = undefined,
	browserBaseUrl = undefined
) {
	const steps = []
	const baseUrl = {
		nodeBaseUrl,
		browserBaseUrl
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
			let anim
			if (typeof subAnimation === 'string') {
				anim = await loadAnimation(`${subAnimation}.js`, baseUrl)
			} else if (typeof animation === 'object' && subAnimation.name) {
				const { name, ...config } = subAnimation
				anim = await loadAnimation(`${name}.js`, Object.assign({}, config, baseUrl))
			}
			step.anims.push((chart, data, assets) => anim(chart, data, assets))
		}
		steps.push(step)
	}
	return steps
}
