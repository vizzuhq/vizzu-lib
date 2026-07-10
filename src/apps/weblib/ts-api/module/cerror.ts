import { CPointer, CVizzu } from '../cvizzu.types'

export class CError extends Error {
	constructor(ptr: CPointer, wasm: CVizzu) {
		const info = new wasm.ExceptionInfo(ptr)
		const type = info.get_type()
		const cMessage = wasm._vizzu_errorMessage(ptr, type)
		const message = wasm.UTF8ToString(cMessage)
		const destructor = info.get_destructor()
		if (destructor) wasm.getWasmTableEntry(destructor)(ptr)
		wasm.___cxa_free_exception(ptr)
		super('error: ' + message)
		this.name = 'CError'
	}
}
