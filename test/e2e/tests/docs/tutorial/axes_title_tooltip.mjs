import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps(
	'../../../../docs/assets/data/music_data.js',
	'axes_title_tooltip'
)

export default testSteps
