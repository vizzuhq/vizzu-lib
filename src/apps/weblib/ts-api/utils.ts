export function recursiveCopy<T>(value: T, Ignore?: new (...args: any[]) => any): T {
  if (value === null || typeof value !== 'object') {
    return value
  }

  if (value instanceof Function) {
    return value
  }

  if (Array.isArray(value) && value instanceof Array) {
    return value.map((item) => recursiveCopy(item, Ignore)) as T
  }

  if (Ignore && value instanceof Ignore) {
    return value
  }

  const copyObj = {} as T
  for (const key in value) {
    if (key in value) {
      copyObj[key] = recursiveCopy(value[key], Ignore)
    }
  }
  return copyObj
}

type Visitor = (path: string, value: any) => void

export function iterateObject<T>(obj: T, paramHandler: Visitor, path: string = ''): void {
  if (obj && obj !== null && typeof obj === 'object') {
    Object.keys(obj).forEach((key) => {
      const newPath = path + (path.length === 0 ? '' : '.') + key
      const value = obj[key as keyof T]
      if (value !== null && typeof value === 'object') {
        iterateObject(value, paramHandler, newPath)
      } else {
        paramHandler(newPath, value)
      }
    })
  }
}

function setNestedProp<T>(obj: T, path: string, value: string): void {
  const propList = path.split('.')
  let currentObj: any = obj
  propList.forEach((prop, i) => {
    if (i < propList.length - 1) {
      currentObj[prop] = currentObj[prop] || (typeof propList[i + 1] === 'number' ? [] : {})
      currentObj = currentObj[prop]
    } else {
      currentObj[prop] = value
    }
  })
}

type Lister = () => string[]
type Getter = (path: string) => string

export function cloneObject<T>(lister: Lister, getter: Getter): Readonly<T> {
  const list = lister()
  const res = {} as T
  for (const path of list) {
    const value = getter(path)
    setNestedProp(res, path, value)
  }
  Object.freeze(res)
  return res
}
