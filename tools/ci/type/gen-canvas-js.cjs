const path = require('path')
const prettier = require('prettier')
const Generator = require('./gen-simple.cjs')

const generator = new Generator({
	prefix: (_name) => 'mergeInto(LibraryManager.library, {\n',
	procParams: (params) => params,
	toParameter: (param, _type) => param,
	toDeclaration: (method, params) =>
		`  canvas_${method}: function(${
			'canvas' + (params !== '' ? ', ' + params : '')
		}) { Module.canvases[canvas].${method}(${params}) }`,
	toContent: (declarations) => declarations.join(',\n'),
	suffix: () => `\n})\n`,
	formatCode: async (code) => {
		const cfg = await prettier.resolveConfig(__dirname)
		return await prettier.format(code, { ...cfg, parser: 'babel' })
	}
})

const outputFile = path.join(__dirname, '../../..', 'src/apps/weblib/canvas.js')

generator.generate(outputFile)
