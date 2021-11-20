const path = require("path");
const yargs = require("yargs");

const TestSuite = require("./modules/integration-test/test-suite.js");


const catchError = (err => {
    process.exitCode = 1;
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    if (testConsole) {
        testConsole.log("[ " + "ERROR".padEnd(testConsole.testStatusPad, " ") + " ] " + errMsg);
    } else {
        console.log("[ ERROR ] " + errMsg);
    }
});


try {
    var argv = yargs

        .usage("Usage: $0 [tests] [options]" + 
                    "\n\nThe objective of the integration test is to E2E test the Vizzu library." + 
                    "\nThe test cases are animations, each animation consists of an animate function call promise chain." + 
                    "\nThe test animations run in Chrome using ChromeDriver and Selenium Webdriver." + 
                    "\nA test case seeks through each animate function calls with a predefined animation step." +
                    "\nA hash is calculated on every created canvas image data for each animation step." + 
                    "\nA test validates a hash calculated from the created hash list during each test case.")

        .help("h")
        .alias("h", "help")

        .version("0.1.0")
        .alias("v", "version")

        .string("ref")
        .nargs("ref", 1)
        .describe("ref", 
                    "Change reference hash list file path of test cases" + 
                    "\n(relative or absolute path where the repo folder is the root)" + 
                    "\n")
        .default("ref", 
                    "/test/integration/test_cases/ref_list.json")

        .string("cases")
        .nargs("cases", 1)
        .describe("cases", 
                    "Change folder path of test cases" + 
                    "\n(relative or absolute path where the repo folder is the root)" + 
                    "\n")
        .default("cases", 
                    "/test/integration/test_cases")

        .choices("images", 
                    ["ALL", "FAILED", "DISABLED"])
        .describe("images", 
                    "Change report images saving behavior" + 
                    "\n- \"ALL\": Create images for every test" + 
                    "\n- \"FAILED\": Create images for failed/warning tests only" + 
                    "\n- \"DISABLED\": Do not create images")
        .default("images", "FAILED")

        .choices("hashes", 
                    ["ALL", "FAILED", "DISABLED"])
        .describe("hashes", 
                    "Change report hashes saving behavior" + 
                    "\n- \"ALL\": Write hashes into the report file for every test" + 
                    "\n- \"FAILED\": Write hashes into the report file for failed/warning tests only" + 
                    "\n- \"DISABLED\": Do not create report file")
        .default("hashes", "FAILED")

        .boolean("nologs")
        .describe("nologs", 
                    "\n Do not save browser and console log into file" + 
                    "\n")
        .default("nologs", false)

        .string("vizzu")
        .nargs("vizzu", 1)
        .describe("vizzu", 
                    "Change Vizzu url" + 
                    "\n(can be forced to use vizzu.js or vizzu.min.js if its given)" + 
                    "\n\n- \"head\": select the last stable Vizzu from the main branch" + 
                    "\n(default: vizzu.min.js)" + 
                    "\n\n- [sha]: select Vizzu with a short commit number" + 
                    "\n(default: vizzu.min.js)" + 
                    "\n\n- [version]: select Vizzu with a version number" + 
                    "\n(vizzu.min.js only)" + 
                    "\n\n- path: select Vizzu from the local file system" + 
                    "\n(relative or absolute path where the repo folder is the root)" +  
                    "\n(default: vizzu.js)" + 
                    "\n")
        .default("vizzu", 
                    "/example/lib/vizzu.js")

        .boolean("g")
        .alias("g", "gui")
        .describe("g", 
                    "Use browser with graphical user interface" + 
                    "\n")
        .default("g", false)

        .number("b")
        .alias("b", "browsers")
        .describe("b", 
                    "Change number of parallel browser windows" + 
                    "\n")
        .default("b", 4)


        .boolean("del")
        .alias("del", "delete")
        .describe("del", 
                    "Delete test report folder" + 
                    "\n")
        .default("del", false)

        .example([
                    ["$0", 
                        "Run all tests from the test cases folder"],
                    ["$0 area_carte_2_polar.mjs", 
                        "Select test case with name"],
                    ["$0 area_carte_2_polar", 
                        "Select test case with name (without extension)"],
                    ["$0 area_carte_2_polar rectangle_carte_2_polar", 
                        "Select test cases with name"],
                    ["$0 basic_animations/coordsystems/area_carte_2_polar.mjs", 
                        "Select test case with path"],
                    ["$0 ./test_cases/basic_animations/coordsystems/*", 
                        "Select test cases with glob pattern"]
                ])
        .example("$0 -g", 
                    "Run all tests and use browser with graphical user interface")
        .example([
                    ["$0 -vizzu head", 
                        "Run all tests with the latest stable Vizzu from the main branch"],
                    ["$0 -vizzu [sha]/vizzu.js", 
                        "Run all tests and select Vizzu with a short commit number" + 
                        "\nand use vizzu.js instead of the default vizzu.min.js"]
                ])

        .argv;

    if (!argv.del) {
        let testSuite = new TestSuite(  
                                        argv.cases, 
                                        argv.ref, 
                                        argv._, 
                                        argv.vizzu, 
                                        argv.gui, 
                                        argv.browsers, 
                                        argv.nologs,
                                        argv.images, 
                                        argv.hashes
                                    );
        var testConsole = testSuite.cnsl();
        testSuite.test().catch(err => {
            catchError(err);
        });
    } else {
        TestSuite.del();
    }
} catch (err) {
    catchError(err);
}
