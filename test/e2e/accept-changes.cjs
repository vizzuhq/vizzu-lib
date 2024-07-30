const fs = require('fs')
const prettier = require('prettier')

function copyHashes(failHashFile, refHashFile) {
	let failHashData
	try {
		failHashData = JSON.parse(fs.readFileSync(failHashFile))
	} catch (error) {
		return
	}
	const refHashData = JSON.parse(fs.readFileSync(refHashFile))

	for (const testFilename in failHashData.test) {
		if (!refHashData.test[testFilename]) {
			refHashData.test[testFilename] = {
				refs: [failHashData.test[testFilename].refs[0]]
			}
		} else if (refHashData.test[testFilename].refs[0] !== '')
			refHashData.test[testFilename].refs[0] = failHashData.test[testFilename].refs[0]
	}

	const formattedRefHashDataReady = prettier.format(JSON.stringify(refHashData, null, '\t'), {
		parser: 'json',
		tabWidth: 4,
		useTabs: true
	})
	formattedRefHashDataReady.then((formattedRefHashData) => {
		fs.writeFileSync(refHashFile, formattedRefHashData)
	})
}

copyHashes('test_report/results/test_cases/test_cases.json', 'test_cases/test_cases.json')
copyHashes('test_report/results/tests/style_tests/style_tests.json', 'tests/style_tests.json')
copyHashes('test_report/results/tests/features/features.json', 'tests/features.json')
copyHashes('test_report/results/tests/fixes/fixes.json', 'tests/fixes.json')
copyHashes('test_report/results/tests/config_tests/config_tests.json', 'tests/config_tests.json')
