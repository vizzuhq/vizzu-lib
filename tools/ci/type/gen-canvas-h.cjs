const path = require('path')
const Generator = require('./gen-simple.cjs')

const generator = new Generator({
	prefix: (_name) => '#ifndef CANVAS_H\n#define CANVAS_H\n\n' +
		'#include <cstddef>\n\nextern "C" {\n\n',
	procParams: (params) => [['canvas', 'const void *'], ...params],
	toParameter: (_param, type) => `${convertToCType(type)}`,
	toDeclaration: (method, params) => `extern void canvas_${method}(${params});`,
	toContent: (declarations) => declarations.join('\n'),
	suffix: () => `\n\n}\n\n#endif\n`,
	formatCode: async (code) => code
})

function convertToCType(yamlType) {
	if (yamlType.includes('/')) return yamlType.split('/')[1]
	const typeMap = {
		number: 'double',
		'C.CString': 'const char *'
	}
	return typeMap[yamlType] || 'const void *'
}

const outputFile = path.join(__dirname, '../../..', 'src/apps/weblib/canvas.h')

generator.generate(outputFile)
