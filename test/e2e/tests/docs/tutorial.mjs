import MdChart from '../../../../docs/assets/javascripts/mdchart.js'

async function getTestSteps(dataFile, configName) {
	const dataLoaded = await import(dataFile)
	const configLoaded = await import(`../../../../docs/tutorial/${configName}/config.js`)
	const [data, config] = await Promise.all([dataLoaded, configLoaded])

	const animations = (
		await MdChart.loadAnimations(
			config.default,
			`./docs/tutorial/${configName}`,
			`../../../../../../docs/tutorial/${configName}`
		)
	).map((obj) => obj.anims)
	animations.unshift((chart) => chart.animate({ data: data.default }))

	return animations.flat(Infinity)
}

export default getTestSteps
