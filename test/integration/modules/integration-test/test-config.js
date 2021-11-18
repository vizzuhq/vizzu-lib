const config = {
    rootDir: "../",
    collectCoverage: true,
    coverageDirectory: "coverage",
    restoreMocks: true,
    testRegex: "(.*test.js)$",
    transform: {},
    testPathIgnorePatterns: ["node_modules"],
    moduleFileExtensions: ["js"],
    setupFilesAfterEnv: ["jest-extended/all"]
}


module.exports = config;
