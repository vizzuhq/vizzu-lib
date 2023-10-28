const path = require('path')
const { exec } = require('child_process')
const Generator = require('./gen-simple.cjs')

const generator = new Generator({
  prefix: (_name) => '#include <stddef.h>\n\nextern "C" {\n\n',
  procParams: (params) => [['canvas', 'const void *'], ...params],
  toParameter: (_param, type) => `${convertToCType(type)}`,
  toDeclaration: (method, params) => `extern void canvas_${method}(${params});`,
  toContent: (declarations) => declarations.join('\n'),
  suffix: () => `\n}\n`,
  formatCode: async (code) => code,
  formatFile: async (file) => {
    return new Promise((resolve, reject) => {
      exec(`clang-format -i ${file}`, (err, stdout, stderr) => {
        if (err) reject(err)
        else resolve(stdout)
      })
    })
  }
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
