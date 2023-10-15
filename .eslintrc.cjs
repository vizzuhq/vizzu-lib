module.exports = {
  env: {
    browser: true,
    jest: true
  },
  extends: ['standard', 'prettier'],
  root: true,
  parserOptions: {
    ecmaVersion: 'latest'
  },
  overrides: [
    {
      files: ['test/integration/test_cases/**', 'test/integration/test_data/**'],
      rules: {
        camelcase: 'off'
      }
    },
    {
      files: ['*.ts', '*.tsx'],
      extends: ['standard', 'prettier', 'plugin:@typescript-eslint/recommended'],
      parser: '@typescript-eslint/parser',
      plugins: ['@typescript-eslint'],
      rules: {
        '@typescript-eslint/explicit-function-return-type': ['error']
      }
    }
  ],
  ignorePatterns: ['**/dist/**', '!.puppeteerrc.cjs']
}
