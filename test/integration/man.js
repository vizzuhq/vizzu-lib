const yargs = require('yargs')

const ManualServer = require('./tools/manual/server/main.js')

try {
  var usage = `
Usage: $0 [tests] [options]

This program allows for the manual testing of Vizzu. The program provides the following options:

Select Vizzu version in the first column to use for playing animations on the left canvas.
Select Vizzu version in the second column to use for playing animations on the right canvas.
This enables comparison between the two versions and helps identify any differences.

Select a test case in the third column to play.
The animation of the selected test case will be displayed using the chosen Vizzu versions.
`

  var argv = yargs
    .usage('Usage: $0 [tests] [options]')

    .help('h')
    .alias('h', 'help')
    .version(false)

    .alias('p', 'port')
    .describe('p', "Change workspace host's port")
    .nargs('p', 1)
    .default('p', '8080')

    .array('c')
    .alias('c', 'configs')
    .nargs('c', 1)
    .describe(
      'c',
      "Change the list of configuration files' path of the test cases" +
        '\n(relative or absolute path where the repo folder is the root)'
    )
    .default('c', [
      '/test/integration/test_cases/test_cases.json',
      '/test/integration/tests/config_tests.json',
      '/test/integration/tests/style_tests.json',
      '/test/integration/tests/features.json',
      '/test/integration/tests/fixes.json'
    ])

    .example([
      ['$0', 'Select all tests'],
      ['$0 area_carte_2_polar.mjs', 'Select test case with name'],
      ['$0 area_carte_2_polar', 'Select test case with name (without extension)'],
      ['$0 area_carte_2_polar rectangle_carte_2_polar', 'Select test cases with name'],
      ['$0 basic_animations/coordsystems/area_carte_2_polar.mjs', 'Select test case with path'],
      ['$0 ./test_cases/basic_animations/coordsystems/*', 'Select test cases with glob pattern']
    ]).argv

  let manual = new ManualServer(argv.configs, argv._, argv.port)
  manual.run()
} catch (err) {
  process.exitCode = 1
  console.error(err)
}
