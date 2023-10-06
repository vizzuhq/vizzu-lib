module.exports = {
  env: {
    browser: true,
    jest: true
  },
  extends: ['standard', 'prettier'],
  parserOptions: {
    ecmaVersion: 'latest'
  },
  overrides: [
    {
      files: ['test/integration/test_cases/**', 'test/integration/test_data/**'],
      rules: {
        camelcase: 'off'
      }
    }
  ],
  ignorePatterns: ['**/showcases/**', '**/dist/**', '!.puppeteerrc.cjs']
}
