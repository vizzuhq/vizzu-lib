const yargs = require('yargs')

const VizzuVersion = require('./modules/vizzu/vizzu-version.cjs')
const TestSuite = require('./modules/e2e-test/test-suite.cjs')

const catchError = (err) => {
	process.exitCode = 1
	let errMsg = err.toString()
	if (err.stack !== undefined) {
		errMsg = err.stack
	}
	if (cnsl) {
		cnsl.log('[ ' + 'ERROR'.padEnd(cnsl.testStatusPad, ' ') + ' ] ' + errMsg)
	} else {
		console.log('[ ERROR ] ' + errMsg)
	}
}

try {
	const usage = `
Usage: $0 [tests] [options]

The e2e test aims to comprehensively test the Vizzu library by executing animations represented as a promise chain of animate function calls.
Each test case follows predefined animation steps and calculates a hash based on the generated canvas image data for each step.
The test validation compares the calculated hash values with the expected values stored for each test case.

The test offers the ability to generate and save images for every canvas image data, as well as create reference and difference images.
This allows for detailed analysis and comparison of the test output.

During testing, two types of warnings may occur.
Firstly, if a test case lacks a stored reference hash, a warning is issued.
Secondly, if a test case produces a different hash compared to the stored reference, but the reference Vizzu library also generates the same hash,
it indicates that the difference is likely caused by environmental factors such as the operating system or the browser.

Please note that the test require Chrome, ChromeDriver and Selenium Webdriver to be properly configured and available.
`

	const argv = yargs

		.usage(usage)

		.help('h')
		.alias('h', 'help')

		.version(false)
		.boolean('version')
		.alias('v', 'version')
		.describe('v', 'Show version number of Vizzu')

		.array('c')
		.alias('c', 'configs')
		.nargs('c', 1)
		.describe(
			'c',
			"Change the list of configuration files' path of the test cases" +
				'\n(relative or absolute path where the repo folder is the root)'
		)
		.default('c', [
			'/test/e2e/test_cases/test_cases.json',
			'/test/e2e/tests/config_tests.json',
			'/test/e2e/tests/style_tests.json',
			'/test/e2e/tests/features.json',
			'/test/e2e/tests/fixes.json',
			'/test/e2e/tests/tickets.json',
			'/test/e2e/tests/docs.json'
		])

		.choices('Werror', ['noref', 'sameref'])
		.describe(
			'Werror',
			'Select warnings to be treated as errors during the test execution' +
				'\n- "noref": Test cases without reference hashes' +
				'\n- "sameref": Test cases that produce the same hashes with the reference Vizzu'
		)
		.coerce('Werror', (arg) => {
			if (arg === true) {
				return ['noref', 'sameref']
			}
			return arg
		})

		.choices('images', ['ALL', 'FAILED', 'DISABLED'])
		.describe(
			'images',
			'Change the saving behavior of images, which are captured from every test steps' +
				'\n- "ALL": Create images for every test' +
				'\n- "FAILED": Create images for failed/warning tests only' +
				'\n- "DISABLED": Do not create images'
		)
		.default('images', 'FAILED')

		.describe(
			'maxFailedImages',
			'The maximum number of failed and warning test cases, of which images are taken'
		)
		.default('maxFailedImages', null)

		.choices('hashes', ['ALL', 'FAILED', 'FAILURES', 'DISABLED'])
		.describe(
			'hashes',
			'Change the saving behavior of hashes, which are unique identifiers calculated for each test case' +
				'\n- "ALL": Write hashes into the report file for every test' +
				'\n- "FAILED": Write hashes into the report file for failed tests only' +
				'\n- "FAILURES": Write hashes into the report file for failed/warning tests only' +
				'\n- "DISABLED": Do not create report file'
		)
		.default('hashes', 'FAILURES')

		.boolean('nologs')
		.describe('nologs', 'Disable the saving of browser and console logs into a log file')
		.default('nologs', false)

		.string('vizzu')
		.nargs('vizzu', 1)
		.describe(
			'vizzu',
			'Change Vizzu url' +
				'\n(can be forced to use vizzu.js or vizzu.min.js if its given)' +
				'\n\n- "head": specify "head" to select the last stable version of Vizzu from the main branch' +
				'\n(default: vizzu.min.js)' +
				'\n\n- [sha]: select Vizzu with a short commit number' +
				'\n(default: vizzu.min.js)' +
				'\n\n- [version]: select Vizzu with a version number' +
				'\n(vizzu.min.js only)' +
				'\n\n- path: select Vizzu from the local file system' +
				'\n(relative or absolute path where the repo folder is the root)' +
				'\n(default: vizzu.js)'
		)
		.default('vizzu', '/dist/vizzu.js')

		.string('vizzu-ref')
		.nargs('vizzu-ref', 1)
		.describe(
			'vizzu-ref',
			'Change reference Vizzu url' +
				'\n(can be forced to use vizzu.js or vizzu.min.js if its given)' +
				'\n\n- "head": specify "head" to select the last stable version of Vizzu from the main branch' +
				'\n(default: vizzu.min.js)' +
				'\n\n- [sha]: select Vizzu with a short commit number' +
				'\n(default: vizzu.min.js)' +
				'\n\n- [version]: select Vizzu with a version number' +
				'\n(vizzu.min.js only)' +
				'\n\n- path: select Vizzu from the local file system' +
				'\n(relative or absolute path where the repo folder is the root)' +
				'\n(default: vizzu.js)'
		)
		.default('vizzu-ref', 'head')

		.boolean('g')
		.alias('g', 'gui')
		.describe('g', 'Use browser with graphical user interface')
		.default('g', false)

		.number('b')
		.alias('b', 'browsers')
		.describe('b', 'Change number of parallel browser windows')
		.default('b', 6)

		.boolean('d')
		.alias('d', 'delete')
		.describe('d', 'Delete test report folder')
		.default('d', false)

		.example([
			['$0', 'Run all tests'],
			['$0 area_carte_2_polar.mjs', 'Select test case with name'],
			['$0 area_carte_2_polar', 'Select test case with name (without extension)'],
			['$0 area_carte_2_polar rectangle_carte_2_polar', 'Select test cases with name'],
			[
				'$0 basic_animations/coordsystems/area_carte_2_polar.mjs',
				'Select test case with path'
			],
			[
				'$0 ./test_cases/basic_animations/coordsystems/*',
				'Select test cases with glob pattern'
			]
		])
		.example([
			['$0 --vizzu head', 'Run all tests with the latest stable Vizzu from the main branch'],
			['$0 --vizzu [x.y.z]', 'Run all tests and select Vizzu with a version number'],
			[
				'$0 --vizzu [sha]/vizzu.js',
				'Run all tests and select Vizzu with a short commit number' +
					'\nand use vizzu.js instead of the default vizzu.min.js'
			]
		]).argv

	if (argv.version) {
		VizzuVersion.getVizzuUrlVersion(argv.vizzu).then((vizzuVersion) => {
			console.log(vizzuVersion)
		})
	} else if (argv.delete) {
		TestSuite.del()
	} else {
		const testSuite = new TestSuite(
			argv.configs,
			argv._,
			argv.nologs,
			argv.browsers,
			argv.gui,
			argv.vizzu,
			argv.vizzuRef,
			argv.Werror || [],
			argv.images,
			argv.maxFailedImages,
			argv.hashes
		)
		// eslint-disable-next-line no-var
		var cnsl = testSuite.cnsl()
		testSuite.test().catch((err) => {
			catchError(err)
		})
	}
} catch (err) {
	catchError(err)
}
