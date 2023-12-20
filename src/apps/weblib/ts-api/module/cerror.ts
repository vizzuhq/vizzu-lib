import { CPointer, CVizzu } from '../cvizzu.types'

export class CError extends Error {
	constructor(ptr: CPointer, wasm: CVizzu) {
		const type = new wasm.ExceptionInfo(ptr).get_type()
		const cMessage = wasm._vizzu_errorMessage(ptr, type)
		const message = wasm.UTF8ToString(cMessage)
		super('error: ' + message)
		this.name = 'CError'
	}
}
