import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps('../../../../docs/assets/data/music_data.js', 'chart_layout')

export default testSteps
