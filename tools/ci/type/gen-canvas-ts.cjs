const prettier = require('prettier')
const path = require('path')
const Generator = require('./gen-simple.cjs')

const generator = new Generator({
	prefix: (name) =>
		`import * as C from '../cvizzu.types.js'\n` + `\n` + `export interface ${name} {\n`,
	procParams: (params) => params,
	toParameter: (param, type) => `${param}: ${convertToTSType(type)}`,
	toDeclaration: (method, params) => `  ${method}(${params}): void`,
	toContent: (declarations) => declarations.join('\n'),
	suffix: () => `\n}\n`,
	formatCode: async (code) => {
		const cfg = await prettier.resolveConfig(__dirname)
		return await prettier.format(code, Object.assign(cfg, { parser: 'typescript' }))
	}
})

function convertToTSType(yamlType) {
	if (yamlType.includes('/')) return yamlType.split('/')[0]
	return yamlType
}

const outputFile = path.join(__dirname, '../../..', 'src/apps/weblib/ts-api/module/canvas.ts')

generator.generate(outputFile)
