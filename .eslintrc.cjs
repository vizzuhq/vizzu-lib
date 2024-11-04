module.exports = {
	overrides: [
		{
			files: ['*.ts', '*.tsx'],
			extends: ['@vizzu/eslint-config/typescript'],
			rules: {
				'no-use-before-define': 'off',
				'@typescript-eslint/no-use-before-define': 'error',
				'@typescript-eslint/explicit-function-return-type': ['error']
			}
		},
		{
			files: ['*.js', '*.mjs', '*.cjs'],
			extends: ['@vizzu/eslint-config/standard']
		},
		{
			files: ['docs/tutorial/shorthands_store/03_b.js'],
			extends: ['@vizzu/eslint-config/standard'],
			rules: {
				'no-var': 'off',
				'no-unused-vars': 'off'
			}
		},
		{
			files: ['test/e2e/test_cases/**', 'test/e2e/test_data/**'],
			extends: ['@vizzu/eslint-config/standard'],
			rules: {
				camelcase: 'off'
			}
		}
	],
	ignorePatterns: ['**/dist/**']
}
