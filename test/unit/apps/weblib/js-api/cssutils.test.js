import {
	isAccessibleStylesheet,
	getCSSCustomProps,
	getCSSCustomPropsForElement,
	propSet,
	propsToObject
} from '../../../../../dist/plugins/cssutils.js'

class CSSStyleRule {
	constructor({ type, style }) {
		this.type = type
		this.style = style
	}
}

const propGet = (obj, path) => {
	return path.reduce((acc, part) => acc?.[part], obj)
}

describe('cssutils.isAccessibleStylesheet()', () => {
	test('true -> can access `.cssRules`', () => {
		const stylesheet = { cssRules: [] }
		expect(isAccessibleStylesheet(stylesheet)).toBeTruthy()
	})

	test('false -> cannot access `.cssRules`', () => {
		const stylesheet = {
			get cssRules() {
				throw new Error('not accessible')
			}
		}
		expect(isAccessibleStylesheet(stylesheet)).toBeFalsy()
	})
})

describe('cssutils.getCSSCustomProps()', () => {
	test('no stylesheet, no props', () => {
		global.document = { styleSheets: [] } // mockig document
		expect(getCSSCustomProps('test')).toEqual([])
	})
	test('empty stylesheets, no props', () => {
		global.document = { styleSheets: [{ cssRules: [] }, { cssRules: [] }] }
		expect(getCSSCustomProps('test')).toEqual([])
	})
	test('stylesheet with proper rules, props expected', () => {
		global.document = {
			styleSheets: [
				{
					cssRules: [
						new CSSStyleRule({
							type: 1,
							style: ['--test-property', '--test-property-2']
						})
					]
				}
			]
		}
		expect(getCSSCustomProps('test')).toEqual(['--test-property', '--test-property-2'])
	})
	test('stylesheet with proper rules, using prefix, props expected', () => {
		global.document = {
			styleSheets: [
				{
					cssRules: [
						new CSSStyleRule({
							type: 1,
							style: ['--test-property', '--no-test-property']
						})
					]
				}
			]
		}
		expect(getCSSCustomProps('test')).toEqual(['--test-property'])
	})
})

describe('cssutils.getCSSCustomPropsForElement()', () => {
	test('only element related props should show up', () => {
		global.document = {
			styleSheets: [
				{
					cssRules: [
						new CSSStyleRule({
							type: 1,
							style: ['--test-property', '--test-property-2']
						})
					]
				}
			]
		}
		global.getComputedStyle = () => {
			return {
				getPropertyValue: (prop) => {
					if (prop === '--test-property') {
						return 'test'
					}
					return ''
				}
			}
		}
		expect(getCSSCustomPropsForElement('whatever', 'test')).toEqual([
			['--test-property', 'test']
		])
	})
})

describe('cssutils.propSet()', () => {
	test('set embedded property on empty object', () => {
		const obj = {}
		propSet(obj, ['alma', 'beka', 'cica'], 'test')
		expect(obj?.alma?.beka?.cica).toEqual('test')
	})

	test('does not overwrite by default', () => {
		const obj = { alma: { beka: { cica: 'notest' } } }
		propSet(obj, ['alma', 'beka', 'cica'], 'test')
		expect(obj?.alma?.beka?.cica).toEqual('notest')
	})

	test('can overwrite if requested', () => {
		const obj = { alma: { beka: { cica: 'notest' } } }
		propSet(obj, ['alma', 'beka', 'cica'], 'test', true)
		expect(obj?.alma?.beka?.cica).toEqual('test')
	})
})

describe('cssutils.propGet()', () => {
	test('get embedded property', () => {
		const obj = { alma: { beka: { cica: 'test' } } }
		expect(propGet(obj, ['alma', 'beka', 'cica'])).toEqual('test')
	})
})

describe('cssutils.propsToObject()', () => {
	test('generate "deep" object from property list', () => {
		const props = [['--alma-beka-cica', 'test']]
		const obj = propsToObject(props, null)
		expect(propGet(obj, ['alma', 'beka', 'cica'])).toEqual('test')
	})

	test('generate "deep" object from property list, using prefix', () => {
		const props = [['--test-alma-beka-cica', 'test']]
		const obj = propsToObject(props, null, 'test')
		expect(propGet(obj, ['alma', 'beka', 'cica'])).toEqual('test')
	})

	test('fill existing "deep" object from property list, using prefix, no overwrite', () => {
		const props = [['--test-alma-beka-cica', 'test']]
		const obj = { alma: { beka: { cica: 'notest' } } }
		propsToObject(props, obj, 'test')
		expect(propGet(obj, ['alma', 'beka', 'cica'])).toEqual('notest')
	})

	test('fill existing "deep" object from property list, using prefix, overwrite', () => {
		const props = [['--test-alma-beka-cica', 'test']]
		const obj = { alma: { beka: { cica: 'notest' } } }
		propsToObject(props, obj, 'test', true)
		expect(propGet(obj, ['alma', 'beka', 'cica'])).toEqual('test')
	})
})
