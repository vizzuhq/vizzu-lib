import MdChart from '../../../../../docs/assets/javascripts/mdchart.js'

const dataLoaded = await import('../../../../../docs/assets/data/music_data.js')
const configLoaded = await import('../../../../../docs/tutorial/axes_title_tooltip/config.js')
const [data, config] = await Promise.all([dataLoaded, configLoaded])

const animations = await MdChart.loadAnimations(
	config.default,
	'./docs/tutorial/axes_title_tooltip',
	'../../../../../../docs/tutorial/axes_title_tooltip'
)
animations.unshift((chart) => chart.animate({ data: data.default }))

const testSteps = animations.flat(Infinity)

export default testSteps
