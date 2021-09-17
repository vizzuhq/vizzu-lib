module.exports = {
    rootDir: "../../",
    roots: [
        '<rootDir>/src',
        '<rootDir>/test/unit',
    ],
    collectCoverage: true,
    coverageProvider: "v8",
    coverageDirectory: "<rootDir>/test/unit/coverage",
    testRegex: "(test/unit/.*(\\.|/)(test|spec))\\.(jsx?|js?|tsx?|ts?)$",
    transform: {
      "^.+\\.jsx?$": "<rootDir>/test/unit/node_modules/babel-jest",
      "^.+\\.mjs$": "<rootDir>/test/unit/node_modules/babel-jest",
    },
    testPathIgnorePatterns: ["<rootDir>/test/unit/node_modules/"],
    moduleFileExtensions: ["js", "jsx", "mjs"]
}