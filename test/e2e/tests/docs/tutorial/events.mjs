import MdChart from '../../../../../docs/assets/javascripts/mdchart.js'

async function getTestSteps(dataFile, configName) {
	const dataLoaded = import(dataFile)
	const configLoaded = import(`../../../../../docs/tutorial/${configName}/config.js`)
	const [data, config] = await Promise.all([dataLoaded, configLoaded])

	const baseUrl = {
		nodeBaseUrl: `./docs/tutorial/${configName}`,
		browserBaseUrl: `../../../../../docs/tutorial/${configName}`
	}

	const steps = []
	for (const animation of config.default) {
		for (const subAnimation of animation.anims) {
			const func = await MdChart.loadAnimation(
				`${subAnimation.name}.js`,
				Object.assign({}, subAnimation, baseUrl)
			)
			steps.push((chart) => func(chart, data.default, animation?.assets))
		}
	}

	steps.unshift((chart) => chart.animate({ data: data.default }))
	return steps
}

const testSteps = await getTestSteps('../../../../../docs/assets/data/music_data.js', 'events')
export default testSteps
