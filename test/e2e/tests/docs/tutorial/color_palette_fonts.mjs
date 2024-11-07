import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps(
	'../../../../docs/assets/data/music_data.js',
	'color_palette_fonts'
)

export default testSteps
