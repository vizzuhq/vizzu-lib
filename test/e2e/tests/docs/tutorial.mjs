import { loadAnimations } from '../../../../docs/assets/javascripts/snippet.js'

async function getTestSteps(dataFile, configName) {
	const dataLoaded = import(dataFile)
	const configLoaded = import(`../../../../docs/tutorial/${configName}/config.js`)
	const [data, config] = await Promise.all([dataLoaded, configLoaded])

	const animations = (
		await loadAnimations(
			config.default,
			`./docs/tutorial/${configName}`,
			`../../../../../../docs/tutorial/${configName}`
		)
	).map((obj) => obj.anims)
	animations.unshift((chart) => chart.animate({ data: data.default }))

	return animations.flat(Infinity)
}

export default getTestSteps
