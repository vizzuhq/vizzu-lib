const path = require("path");
const yargs = require("yargs");

const VizzuVersion = require("./modules/vizzu/vizzu-version");
const TestSuite = require("./modules/integration-test/test-suite.js");

const catchError = (err) => {
  process.exitCode = 1;
  let errMsg = err.toString();
  if (err.stack !== undefined) {
    errMsg = err.stack;
  }
  if (cnsl) {
    cnsl.log("[ " + "ERROR".padEnd(cnsl.testStatusPad, " ") + " ] " + errMsg);
  } else {
    console.log("[ ERROR ] " + errMsg);
  }
};

try {
  var argv = yargs

    .usage(
      "Usage: $0 [tests] [options]" +
        "\n\nThe objective of the integration test is to E2E test the Vizzu library." +
        "\nThe test cases are animations, each animation consists of an animate function call promise chain." +
        "\nThe test animations run in Chrome using ChromeDriver and Selenium Webdriver." +
        "\nA test case seeks through each animate function calls with a predefined animation step." +
        "\nA hash is calculated on every created canvas image data for each animation step." +
        "\nA test validates a hash calculated from the created hash list during each test case."
    )

    .help("h")
    .alias("h", "help")

    .version(false)
    .boolean("version")
    .alias("v", "version")
    .describe("v", "Show version number of Vizzu")

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

    .choices("images", ["ALL", "FAILED", "DISABLED"])
    .describe(
      "images",
      "Change report images saving behavior" +
        '\n- "ALL": Create images for every test' +
        '\n- "FAILED": Create images for failed/warning tests only' +
        '\n- "DISABLED": Do not create images'
    )
    .default("images", "FAILED")

    .choices("hashes", ["ALL", "FAILED", "DISABLED"])
    .describe(
      "hashes",
      "Change report hashes saving behavior" +
        '\n- "ALL": Write hashes into the report file for every test' +
        '\n- "FAILED": Write hashes into the report file for failed/warning tests only' +
        '\n- "DISABLED": Do not create report file'
    )
    .default("hashes", "FAILED")

    .boolean("nologs")
    .describe(
      "nologs",
      "\n Do not save browser and console log into file" + "\n"
    )
    .default("nologs", false)

    .string("vizzu")
    .nargs("vizzu", 1)
    .describe(
      "vizzu",
      "Change Vizzu url" +
        "\n(can be forced to use vizzu.js or vizzu.min.js if its given)" +
        '\n\n- "head": select the last stable Vizzu from the main branch' +
        "\n(default: vizzu.min.js)" +
        "\n\n- [sha]: select Vizzu with a short commit number" +
        "\n(default: vizzu.min.js)" +
        "\n\n- [version]: select Vizzu with a version number" +
        "\n(vizzu.min.js only)" +
        "\n\n- path: select Vizzu from the local file system" +
        "\n(relative or absolute path where the repo folder is the root)" +
        "\n(default: vizzu.js)" +
        "\n"
    )
    .default("vizzu", "/example/lib/vizzu.js")

    .boolean("g")
    .alias("g", "gui")
    .describe("g", "Use browser with graphical user interface" + "\n")
    .default("g", false)

    .number("b")
    .alias("b", "browsers")
    .describe("b", "Change number of parallel browser windows" + "\n")
    .default("b", 3)

    .boolean("d")
    .alias("d", "delete")
    .describe("d", "Delete test report folder" + "\n")
    .default("d", false)

    .example([
      ["$0", "Run all tests"],
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
    ])
    .example(
      "$0 -g",
      "Run all tests and use browser with graphical user interface"
    )
    .example([
      [
        "$0 -vizzu head",
        "Run all tests with the latest stable Vizzu from the main branch",
      ],
      [
        "$0 -vizzu [sha]/vizzu.js",
        "Run all tests and select Vizzu with a short commit number" +
          "\nand use vizzu.js instead of the default vizzu.min.js",
      ],
    ]).argv;

  if (argv.version) {
    VizzuVersion.getVizzuUrlVersion(argv.vizzu).then((vizzuVersion) => {
      console.log(vizzuVersion);
    });
  } else if (argv.delete) {
    TestSuite.del();
  } else {
    let testSuite = new TestSuite(
      argv.configs,
      argv._,
      argv.nologs,
      argv.browsers,
      argv.gui,
      argv.vizzu,
      argv.images,
      argv.hashes
    );
    var cnsl = testSuite.cnsl();
    testSuite.test().catch((err) => {
      catchError(err);
    });
  }
} catch (err) {
  catchError(err);
}
