export function recursiveCopy(obj) {
  if (obj === null || typeof obj !== 'object') {
    return obj
  }

  if (obj instanceof Function) {
    return obj
  }

  if (obj instanceof Array) {
    const copyArray = []
    obj.map((arrayElement) => copyArray.push(arrayElement))
    return copyArray
  }

  const copyObj = {}
  for (const key in obj) {
    if (key in obj) {
      copyObj[key] = recursiveCopy(obj[key])
    }
  }
  return copyObj
}
