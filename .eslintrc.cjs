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
			files: ['test/integration/test_cases/**', 'test/integration/test_data/**'],
			extends: ['@vizzu/eslint-config/standard'],
			rules: {
				camelcase: 'off'
			}
		}
	],
	ignorePatterns: ['**/dist/**']
}
