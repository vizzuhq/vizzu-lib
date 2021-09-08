module.exports = {
    testRegex: "(/__tests__/.*|(\\.|/)(test|spec))\\.(jsx?|js?|tsx?|ts?)$",
    transform: {
      "^.+\\.jsx?$": "babel-jest",
      "^.+\\.mjs$": "babel-jest",
    },
    testPathIgnorePatterns: ["<rootDir>/node_modules/"],
    moduleFileExtensions: ["js", "jsx", "mjs"]
  }