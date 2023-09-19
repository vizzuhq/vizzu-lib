module.exports = {
    env: {
        browser: true,
        jest: true,
    },
    extends: [
      'standard',
      'prettier'
    ],
    parserOptions: {
      ecmaVersion: 'latest'
    },
    rules: {
      camelcase: 0
    }
  }