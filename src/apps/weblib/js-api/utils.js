export function recursiveCopy(value, Ignore) {
  if (value === null || typeof value !== 'object') {
    return value
  }
  if (value instanceof Function) {
    return value
  }
  if (Array.isArray(value) && value instanceof Array) {
    return value.map((item) => recursiveCopy(item, Ignore))
  }
  if (Ignore && value instanceof Ignore) {
    return value
  }
  const copyObj = {}
  for (const key in value) {
    if (key in value) {
      copyObj[key] = recursiveCopy(value[key], Ignore)
    }
  }
  return copyObj
}
export function iterateObject(obj, paramHandler, path = '') {
  if (obj && obj !== null && typeof obj === 'object') {
    Object.keys(obj).forEach((key) => {
      const newPath = path + (path.length === 0 ? '' : '.') + key
      const value = obj[key]
      if (value !== null && typeof value === 'object') {
        iterateObject(value, paramHandler, newPath)
      } else {
        paramHandler(newPath, value)
      }
    })
  }
}
function isObject(value) {
  return typeof value === 'object' && value !== null
}
function setNestedProp(obj, path, value) {
  const propList = path.split('.')
  let currentObj = obj
  propList.forEach((prop, i) => {
    if (!isObject(currentObj)) {
      throw new Error(`Expected an object, but got ${typeof currentObj}`)
    }
    if (i < propList.length - 1) {
      currentObj[prop] = currentObj[prop] || (typeof propList[i + 1] === 'number' ? [] : {})
      currentObj = currentObj[prop]
    } else {
      currentObj[prop] = value
    }
  })
}
export function cloneObject(lister, getter) {
  const list = lister()
  const res = {}
  for (const path of list) {
    const value = getter(path)
    setNestedProp(res, path, value)
  }
  return Object.freeze(res)
}
