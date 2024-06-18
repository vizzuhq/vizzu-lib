import { CPointer } from '../cvizzu.types'

export type FnFree = (rawCPointer: CPointer) => void

export type CPointerClosure = () => CPointer

export class ObjectRegistry {
	private _finalizationRegistry: FinalizationRegistry<CPointer>

	constructor(fnFree: FnFree) {
		this._finalizationRegistry = new FinalizationRegistry((cPointer: CPointer) => {
			fnFree(cPointer)
		})
	}

	register(objectId: CPointerClosure): void {
		this._finalizationRegistry.register(objectId, objectId())
	}

	unregister(objectId: CPointerClosure): void {
		this._finalizationRegistry.unregister(objectId)
	}
}
