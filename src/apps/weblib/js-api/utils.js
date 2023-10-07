export function recursiveCopy(value, Ignore) {
  if (value === null || typeof value !== 'object') {
    return value
  }

  if (value instanceof Function) {
    return value
  }

  if (Array.isArray(value)) {
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
  if (obj) {
    Object.keys(obj).forEach((key) => {
      const newPath = path + (path.length === 0 ? '' : '.') + key
      if (obj[key] !== null && typeof obj[key] === 'object') {
        iterateObject(obj[key], paramHandler, newPath)
      } else {
        if (newPath !== '' + newPath) {
          throw new Error('first parameter should be string')
        }
        paramHandler(newPath, obj[key])
      }
    })
  }
}

function setNestedProp(obj, path, value) {
  const propList = path.split('.')
  propList.forEach((prop, i) => {
    if (i < propList.length - 1) {
      obj[prop] = obj[prop] || (typeof propList[i + 1] === 'number' ? [] : {})
      obj = obj[prop]
    } else {
      obj[prop] = value
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
  Object.freeze(res)
  return res
}
