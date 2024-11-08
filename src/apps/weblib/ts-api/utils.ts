import { Mirrored } from './tsutils'

export function recursiveCopy<T>(value: T, Ignore?: new (...args: never[]) => unknown): T {
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

type Visitor = (path: string, value: unknown) => void

export function isIterable(value: unknown): value is Record<string, unknown> {
	return typeof value === 'object' && value !== null
}

export function iterateObject<T>(obj: T, paramHandler: Visitor, path: string = ''): void {
	if (obj && obj !== null && typeof obj === 'object') {
		const isArray = Array.isArray(obj)
		if (isArray) {
			paramHandler(path + '.begin', obj.length)
		}
		Object.keys(obj).forEach((key) => {
			const newPath = path + (path.length === 0 ? '' : '.') + key
			const value = obj[key as keyof T]
			if (isIterable(value)) {
				iterateObject(value, paramHandler, newPath)
			} else {
				paramHandler(newPath, value)
			}
		})
		if (isArray) {
			paramHandler(path + '.end', obj.length)
		}
	}
}

function isObject(value: unknown): value is Record<string, unknown> {
	return typeof value === 'object' && value !== null
}

function setNestedProp<T>(obj: T, path: string, value: unknown): void {
	const propList = path.split('.')
	let currentObj: unknown = obj
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

type Lister = () => string[]
type Getter = (path: string) => unknown

export function mirrorObject<T>(lister: Lister, getter: Getter): Mirrored<T> {
	const list = lister()
	const res = {} as T
	for (const path of list) {
		const value = getter(path)
		setNestedProp(res, path, value)
	}
	return Object.freeze(res) as Mirrored<T>
}
