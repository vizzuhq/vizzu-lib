const path = require('path')

const terser = require('@rollup/plugin-terser')

module.exports = [
  {
    input: path.resolve(__dirname, '../../example/lib/vizzu.js'),
    output: {
      file: path.resolve(__dirname, '../../example/lib/vizzu.min.js'),
      format: 'es',
      name: 'bundle'
    },
    plugins: [terser()]
  }
]
