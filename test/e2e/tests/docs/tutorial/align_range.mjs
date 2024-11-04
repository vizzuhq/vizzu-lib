import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps('../../../../docs/assets/data/music_data.js', 'align_range')

export default testSteps
