const VizzuCloudFunctions = require('./vizzu-cloudfunctions.js')

test('if getRemoteCloudFunctions()', () => {
  expect(VizzuCloudFunctions.getRemoteCloudFunctions()).toBe(
    'https://europe-west3-vizzu-ci.cloudfunctions.net'
  )
})
