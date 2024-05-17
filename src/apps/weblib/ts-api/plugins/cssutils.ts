export const isAccessibleStylesheet = (stylesheet: CSSStyleSheet): boolean => {
	try {
		// eslint-disable-next-line no-unused-expressions
		stylesheet.cssRules
		return true
	} catch (e) {
		return false
	}
}

export const getCSSCustomProps = (pfx = ''): string[] =>
	[...document.styleSheets].filter(isAccessibleStylesheet).reduce<string[]>(
		(finalArr, sheet) =>
			finalArr.concat(
				[...sheet.cssRules]
					.filter(
						(rule: CSSRule): rule is CSSStyleRule =>
							rule.constructor.name === 'CSSStyleRule'
					)
					.reduce<string[]>((propValArr, rule) => {
						const props = [...rule.style]
							.filter(
								(propName: string) =>
									propName.trim().indexOf('--' + pfx + '-') === 0
							)
							.map((propName: string) => propName.trim())
						return [...propValArr, ...props]
					}, [])
			),
		[]
	)

export const getCSSCustomPropsForElement = (el: HTMLElement, pfx = ''): string[][] => {
	const props = getCSSCustomProps(pfx)
	const style = getComputedStyle(el)
	return props
		.map((prop) => [prop, style.getPropertyValue(prop).trim()])
		.filter((pv) => pv[1] !== '')
}

interface StringKeyedObject {
	[key: string]: unknown
}

export const propSet = <T extends StringKeyedObject, V>(
	obj: T,
	path: string[],
	value: V,
	overwrite: boolean
): T => {
	path.reduce((acc: StringKeyedObject, part: string, idx: number) => {
		if (idx === path.length - 1) {
			if (overwrite || !acc[part]) {
				acc[part] = value
			}
		} else if (!acc[part]) {
			acc[part] = {}
		}

		return acc[part] as StringKeyedObject
	}, obj)

	return obj
}

export const propsToObject = <T>(
	props: string[][],
	propObj: T | null | undefined,
	pfx = '',
	overwrite = false
): T => {
	propObj = propObj || ({} as T)
	propObj = props.reduce((obj, [prop, val]) => {
		if (prop) {
			const propname = prop.replace('--' + (pfx ? pfx + '-' : ''), '')
			const proppath = propname.split('-')

			propSet(obj as StringKeyedObject, proppath, val, overwrite)
		}
		return obj
	}, propObj)

	return propObj
}
