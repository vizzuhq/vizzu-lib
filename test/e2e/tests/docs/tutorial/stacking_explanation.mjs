import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps(
	'../../../../docs/tutorial/stacking_explanation/data.js',
	'stacking_explanation'
)

export default testSteps
