import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps(
	'../../../../docs/assets/data/music_data.js',
	'shorthands_store'
)

export default testSteps
