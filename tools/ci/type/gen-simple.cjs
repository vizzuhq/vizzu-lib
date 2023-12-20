const fs = require('fs')
const path = require('path')
const yaml = require('js-yaml')

class Generator {
	constructor(options) {
		this.options = options
		this.inputFile = path.join(__dirname, '../../..', 'src/apps/weblib/canvas.yaml')
	}

	async generate(outputFile) {
		const definition = yaml.load(fs.readFileSync(this.inputFile, 'utf8'))
		const code = this.generateCode(definition)
		const content = await this.options.formatCode(code)
		console.log(`Writing ${outputFile}`)
		fs.writeFileSync(outputFile, content)
	}

	generateCode(def) {
		const ifaceName = Object.keys(def)[0]
		let code = this.options.prefix(ifaceName)
		const declarations = []
		for (const [method, paramsObject] of Object.entries(def[ifaceName])) {
			const params = this.options
				.procParams(Object.entries(paramsObject))
				.map(([param, type]) => this.options.toParameter(param, type))
				.join(', ')
			declarations.push(this.options.toDeclaration(method, params))
		}
		code += this.options.toContent(declarations)
		code += this.options.suffix()
		return code
	}
}

module.exports = Generator
