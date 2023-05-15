const yargs = require("yargs");

const Manual = require("./modules/manual/manual.js");

try {
  var argv = yargs
    .usage("Usage: $0 [tests] [options]")

    .help("h")
    .alias("h", "help")
    .version(false)

    .alias("p", "port")
    .describe("p", "Change workspace host's port")
    .nargs("p", 1)
    .default("p", "8080")

    .array("c")
    .alias("c", "configs")
    .nargs("c", 1)
    .describe(
      "c",
      "Change the list of config file's path of the test cases" +
        "\n(relative or absolute path where the repo folder is the root)" +
        "\n"
    )
    .default("c", [
      "/test/integration/test_cases/test_cases.json",
      "/test/integration/tests/style_tests.json",
    ])

    .example([
      ["$0", "Select all tests"],
      ["$0 area_carte_2_polar.mjs", "Select test case with name"],
      [
        "$0 area_carte_2_polar",
        "Select test case with name (without extension)",
      ],
      [
        "$0 area_carte_2_polar rectangle_carte_2_polar",
        "Select test cases with name",
      ],
      [
        "$0 basic_animations/coordsystems/area_carte_2_polar.mjs",
        "Select test case with path",
      ],
      [
        "$0 ./test_cases/basic_animations/coordsystems/*",
        "Select test cases with glob pattern",
      ],
    ]).argv;

  let manual = new Manual(argv.configs, argv._, argv.port);
  manual.run();
} catch (err) {
  process.exitCode = 1;
  console.error(err);
}
