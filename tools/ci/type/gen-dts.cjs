const fs = require('fs')
const path = require('path')
const prettier = require('prettier')
const yaml = require('js-yaml')

class SchemaCollection {
	constructor(inputDir) {
		this._inputDir = inputDir
		const yamls = this._getFiles('yaml')
		this.schemas = this._getContent(yamls, (content) => yaml.load(content))
	}

	_getFiles(ext) {
		let files = fs.readdirSync(this._inputDir)
		files = files.filter((file) => file.endsWith('.' + ext))
		return files
	}

	_getContent(files, format) {
		const contents = []
		for (const file of files) {
			const basename = file.split('.')[0]
			const formatted = format(fs.readFileSync(this._inputDir + '/' + file, 'utf8'))
			contents[basename] = formatted
		}
		return contents
	}
}

class DTSGenerator {
	constructor() {
		this._export = ''
		this._namespaces = []
		this._imports = []
		this._content = ''
	}

	addContent(content) {
		this._content += content
	}

	async writeFile(outputPath) {
		const cfg = await prettier.resolveConfig(__dirname)
		const formatted = await prettier.format(
			this._content,
			Object.assign(cfg, { parser: 'typescript' })
		)
		const dir = path.dirname(outputPath)
		if (!fs.existsSync(dir)) {
			fs.mkdirSync(dir, { recursive: true })
		}
		fs.writeFileSync(outputPath, formatted)
		this._content = ''
	}

	async generate(schemas, outputDir) {
		this._namespaces = [...Object.keys(schemas)]
		for (const name of this._namespaces) {
			console.log(`Generating ${name}.ts`)
			if (schemas[name]) this.addModule(schemas[name])
			else throw new Error(`Schema ${name} not found`)
			await this.writeFile(outputDir + `/${name}.ts`)
		}
		this._namespaces = []
		this._imports = []
	}

	addModule(schema) {
		if (schema.$import) {
			this.addImports(schema.$import)
		}
		if (schema.$ref) {
			this._export = schema.$ref
		}
		this.addDefinitions(schema.definitions)
		this._export = ''
	}

	addImports(imports) {
		for (const name in imports) {
			const file = imports[name]
			this.addContent(`import * as ${name} from '${file}.js'\n`)
			this._imports.push(name)
		}
		this.addContent('\n')
	}

	addDefinitions(definitions) {
		for (const name in definitions) {
			this.addDefinition(name, definitions[name])
		}
	}

	addDefinition(name, definition) {
		if (definition.description) {
			const comment = this._getComment(definition.description)
			this.addContent(comment + '\n')
		}

		if (definition.type === 'object') {
			this._addInterface(name, definition)
		} else {
			this._addType(name, definition)
		}
		this.addContent('\n')
	}

	_addInterface(name, definition) {
		this._validateType(definition, 'properties', '$extends', 'required')
		const type = name === this._export ? `export default class` : 'export interface'
		if (definition.$extends) {
			const extendsType = this._getExtendsType(name, definition.$extends)
			this.addContent(`${type} ${name} extends ${extendsType} {\n`)
		} else {
			this.addContent(`${type} ${name} {\n`)
		}
		this._vizitProperties(definition)
		this.addContent('}\n')
	}

	_getExtendsType(name, definition) {
		if (typeof definition === 'string') {
			return definition
		} else if (definition instanceof Array) {
			return definition.map((x) => this._getExtendsType(name, x)).join(', ')
		} else if (definition instanceof Object) {
			return this._getType(name, definition)
		} else throw new Error('Invalid extends type')
	}

	_vizitProperties(definition) {
		if (definition.properties) {
			for (const name in definition.properties) {
				const property = definition.properties[name]
				const required = definition.required && definition.required.includes(name)
				this._addProperty(name, property, required)
			}
		}
	}

	_addProperty(name, property, required) {
		if (property.description) {
			const comment = this._getComment(property.description)
			this.addContent(comment + '\n')
		}
		if (property.type === 'function') {
			this._addMethod(name, property)
		} else {
			const type = this._getType(name, property)
			const requiredStr = required ? '' : '?'
			this.addContent(`${name}${requiredStr}: ${type};\n`)
		}
	}

	_addMethod(name, property) {
		let prefix = ''
		if (property.static) prefix += 'static '
		if (property.getter) prefix += 'get '
		const type = this._getType(name, property)
		let args = type.split(' => ')[0]
		const returnType = type.split(' => ')[1]
		if (name === 'constructor') {
			this.addContent(`${prefix}constructor${args};\n`)
		} else if (name === 'operator[]') {
			args = args.replace(/^\(|\)$/g, '')
			this.addContent(`${prefix}[${args}]: ${returnType};\n`)
		} else {
			this.addContent(`${prefix}${name}${args}: ${returnType};\n`)
		}
	}

	_addType(name, definition) {
		const type = this._getType(name, definition)
		this.addContent(`export type ${name} = ${type};\n`)
	}

	_getType(name, definition) {
		const type = this._getRawType(name, definition)
		if (definition.nullable) {
			return `(${type} | null)`
		} else {
			return type
		}
	}

	_getRawType(name, definition) {
		if (definition.type === 'number') {
			this._validateType(definition)
			return 'number'
		} else if (definition.type === 'boolean') {
			this._validateType(definition)
			return 'boolean'
		} else if (definition.type === 'void') {
			this._validateType(definition)
			return 'void'
		} else if (definition.type === 'string') {
			if (definition.enum) {
				this._validateType(definition, 'enum')
				return definition.enum.map((item) => `'${item}'`).join(' | ')
			} else if (definition.mask) {
				this._validateType(definition, 'mask')
				return this._regexpToTemplateLiteral(definition.mask)
			} else {
				this._validateType(definition)
				return 'string'
			}
		} else if (definition.type === 'array') {
			this._validateType(definition, 'items')
			const type = this._getType(name, definition.items)
			return `${type}[]`
		} else if (definition.type === 'function') {
			this._validateType(definition, 'arguments', 'return', 'required', 'getter', 'static')
			let args = ''
			if (definition.arguments) {
				args = Object.entries(definition.arguments)
					.map(([name, def]) => {
						const type = this._getType(name, def)
						const required = definition.required && definition.required.includes(name)
						return `${name}${required ? '' : '?'}: ${type}`
					})
					.join(', ')
			}
			const returns = definition.return ? this._getType(name, definition.return) : 'void'
			return `(${args}) => ${returns}`
		} else if (definition.type === 'any') {
			return 'unknown'
		} else if (definition.$ref) {
			this._validateDef(definition, '$ref', '$template')
			const refType = this._getRef(definition.$ref)
			if (definition.$template) {
				this._validateProperties(definition.$template, 'T')
				const template = this._getRef(definition.$template.T)
				return `${refType}<${template}>`
			} else return refType
		} else if (definition.oneOf) {
			this._validateDef(definition, 'oneOf')
			return '(' + definition.oneOf.map((item) => this._getType(name, item)).join(' | ') + ')'
		} else if (definition.allOf) {
			this._validateDef(definition, 'allOf')
			return '(' + definition.allOf.map((item) => this._getType(name, item)).join(' & ') + ')'
		} else {
			throw new Error(`unknown type: '${definition.type}' in '${definition}'`)
		}
	}

	_getRef(reference) {
		const parts = reference.split(':')
		if (
			parts.length === 1 ||
			this._namespaces.includes(parts[0]) ||
			this._imports.includes(parts[0])
		) {
			parts[0] = this._upperCaseFirstLetter(parts[0])
		} else {
			parts[0] = `import('./${parts[0]}')`
		}
		return parts.join('.')
	}

	_getComment(text) {
		text = text.replace(/\n$/, '')
		return '/** ' + text.split('\n').join('\n    ') + ' */'
	}

	_regexpToTemplateLiteral(regexp) {
		// trim leading and trailing slashes
		regexp = regexp.replace(/^\/|\/$/g, '')
		// de-escape regexp special characters
		regexp = regexp.replace(/\\([-[\]{}()*+?.,\\^$|#\s])/g, '$1')
		// replate :type: with ${type}
		// eslint-disable-next-line no-template-curly-in-string
		regexp = regexp.replace(/:([^:]+):/g, '${$1}')
		return '`' + regexp + '`'
	}

	_validateType(definition, ...additionalProperties) {
		this._validateDef(definition, 'type', ...additionalProperties)
	}

	_validateDef(definition, ...additionalProperties) {
		this._validateProperties(definition, 'description', 'nullable', ...additionalProperties)
	}

	_validateProperties(definition, ...properties) {
		if (Object.keys(definition).filter((key) => !properties.includes(key)).length) {
			throw new Error(`unknown property in: '${JSON.stringify(definition)}'`)
		}
	}

	_upperCaseFirstLetter(string) {
		return string.charAt(0).toUpperCase() + string.slice(1)
	}
}

let inputDir = process.argv[2]
let outputDir = process.argv[3]

if (!inputDir) inputDir = path.join(__dirname, '../../..', 'src/apps/weblib/typeschema-api')
if (!outputDir) outputDir = path.join(__dirname, '../../..', 'src/apps/weblib/ts-api/types')

const collection = new SchemaCollection(inputDir)

const generator = new DTSGenerator()

generator.generate(collection.schemas, outputDir)
