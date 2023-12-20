const path = require('path')

const terser = require('@rollup/plugin-terser')

module.exports = [
	{
		input: path.resolve(__dirname, '../../dist/index.js'),
		output: {
			file: path.resolve(__dirname, '../../dist/vizzu.min.js'),
			format: 'es',
			name: 'bundle'
		},
		plugins: [
			terser({
				mangle: {
					properties: {
						keep_quoted: true,
						regex: /^_/
					}
				}
			})
		]
	}
]
