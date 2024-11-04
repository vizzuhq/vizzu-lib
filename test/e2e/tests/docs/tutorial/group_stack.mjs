import getTestSteps from '../tutorial.mjs'

const testSteps = await getTestSteps('../../../../docs/assets/data/music_data.js', 'group_stack')

export default testSteps
