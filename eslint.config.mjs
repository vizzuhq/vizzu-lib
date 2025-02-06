import { EslintJavaScriptConfig, EslintTypeScriptConfig } from '@vizzu/eslint-config'

export default [
    ...EslintTypeScriptConfig,
    ...EslintJavaScriptConfig,
    {
        files: ['**/*.ts', '**/*.tsx'],
        rules: {
            '@typescript-eslint/no-use-before-define': 'error',
            '@typescript-eslint/explicit-function-return-type': 'error',
            'no-use-before-define': 'off',
            'no-unused-vars': 'off',
            '@typescript-eslint/no-unsafe-member-access': 'off',
            '@typescript-eslint/no-unsafe-assignment': 'off',
            '@typescript-eslint/no-unsafe-return': 'off',
            '@typescript-eslint/no-unsafe-call': 'off',
            '@typescript-eslint/no-unused-vars': 'off',
            '@typescript-eslint/no-redundant-type-constituents': 'off',
            '@typescript-eslint/no-unsafe-argument': 'off',
            '@typescript-eslint/no-unnecessary-type-assertion': 'off',
            '@typescript-eslint/no-unused-expressions': 'off',
            '@typescript-eslint/unbound-method': 'off',
            '@typescript-eslint/restrict-template-expressions': 'off',
            '@typescript-eslint/await-thenable': 'off',
            '@typescript-eslint/no-floating-promises': 'off'
        }
    },
    {
        files: ['test/e2e/test_cases/**', 'test/e2e/test_data/**'],
        rules: {
            'camelcase': 'off'
        }
    },
    {
        files: ['src/**'],
        languageOptions: {
            globals: {
                getValue: true,
                LibraryManager: true,
                mergeInto: true,
                Module: true,
                setValue: true,
                UTF8ToString: true
            }
        }
    },
    {
        files: ['test/**'],
        languageOptions: {
            globals: {
                d3: true,
                GIF: true,
                markerjs2: true,
                mjslive: true,
                Module: true,
                tinycolor: true
            }
        }
    },
    {
        ignores: [
            '**/node_modules/**',
            '**/build/**',
            '**/dist/**',
            '**/.vscode/**',
            '**/*.d.ts',
            'src/apps/weblib/ts-api/types/**'
        ]
    }
]
