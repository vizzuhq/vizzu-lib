const VizzuCloudFunctions = require('./vizzu-cloudfunctions.cjs')

test('if getRemoteCloudFunctions()', () => {
	expect(VizzuCloudFunctions.getRemoteCloudFunctions()).toBe(
		'https://europe-west3-vizzu-ci.cloudfunctions.net'
	)
})
