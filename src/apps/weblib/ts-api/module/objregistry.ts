import { CPointer } from '../cvizzu.types'

export type FnFree = (rawCPointer: CPointer) => void
export type FnGetter = () => CPointer

export type CPointerClosure = () => CPointer

export class ObjectRegistry {
	private _finalizationRegistry: FinalizationRegistry<CPointer>

	constructor(fnFree: FnFree) {
		this._finalizationRegistry = new FinalizationRegistry((cPointer: CPointer) => {
			fnFree(cPointer)
		})
	}

	get(fnGetter: FnGetter): CPointerClosure {
		const cPointer = fnGetter()
		const object = (): CPointer => cPointer
		this._finalizationRegistry.register(object, cPointer)
		return object
	}
}
