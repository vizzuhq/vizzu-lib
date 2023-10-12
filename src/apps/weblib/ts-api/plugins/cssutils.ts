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
          .filter((rule: CSSRule): rule is CSSStyleRule => rule.constructor.name === 'CSSStyleRule')
          .reduce<string[]>((propValArr, rule) => {
            const props = [...rule.style]
              .filter((propName: string) => propName.trim().indexOf('--' + pfx + '-') === 0)
              .map((propName: string) => propName.trim())
            return [...propValArr, ...props]
          }, [])
      ),
    []
  )

export const getCSSCustomPropsForElement = (el: HTMLElement, pfx = '') => {
  const props = getCSSCustomProps(pfx)
  const style = getComputedStyle(el)
  return props
    .map((prop) => [prop, style.getPropertyValue(prop).trim()])
    .filter((pv) => pv[1] !== '')
}

interface LooseObject {
  [key: string]: any
}

export const propSet = (obj: LooseObject, path: string[], value: any, overwrite: boolean) => {
  path.reduce((acc, part, idx) => {
    if (idx === path.length - 1) {
      if (overwrite || !acc[part]) {
        acc[part] = value
      }
    } else if (!acc[part]) {
      acc[part] = {}
    }

    return acc[part]
  }, obj)
  return obj
}

export const propsToObject = (
  props: string[][],
  propObj: LooseObject,
  pfx = '',
  overwrite = false
) => {
  propObj = propObj || {}
  propObj = props.reduce((obj, [prop, val]) => {
    if (prop) {
      const propname = prop.replace('--' + (pfx ? pfx + '-' : ''), '')
      const proppath = propname.split('-')

      propSet(obj, proppath, val, overwrite)
    }
    return obj
  }, propObj)

  return propObj
}
