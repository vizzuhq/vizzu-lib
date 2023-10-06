export function recursiveCopy(value) {
  if (value === null || typeof value !== 'object') {
    return value
  }

  if (value instanceof Function) {
    return value
  }

  if (Array.isArray(value)) {
    return value.map((item) => recursiveCopy(item))
  }

  const copyObj = Object.create(value)
  for (const key in value) {
    if (key in value) {
      copyObj[key] = recursiveCopy(value[key])
    }
  }
  return copyObj
}
