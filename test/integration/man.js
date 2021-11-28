const yargs = require("yargs");

const Manual = require("./modules/manual/manual.js");


try {
    var argv = yargs
        .usage("Usage: [options]")

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
        .describe("c",
            "Change the list of config file's path of the test cases" +
            "\n(relative or absolute path where the repo folder is the root)" +
            "\n")
        .default("c",
            [
                "/test/integration/test_cases/test_cases.json",
                "/test/integration/test_options/test_options.json"
            ])
        .argv;

    let manual = new Manual(argv.configs, argv.port);
    manual.run();
} catch (err) {
    process.exitCode = 1;
    console.error(err);
}
