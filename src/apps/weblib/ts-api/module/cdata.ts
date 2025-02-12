import { CPointer, CRecordPtr } from '../cvizzu.types'
import { CObject, CEnv } from './cenv.js'
import { Mirrored } from '../tsutils'
import * as Data from '../types/data.js'

export class CRecord extends CObject {
	constructor(env: CEnv, recordPtr: CRecordPtr) {
		super(() => recordPtr, env)
	}

	getValue(columnName: string): string | number {
		const col = this._toCString(columnName)
		let ptr
		let value

		try {
			ptr = this._call(this._wasm._record_getValue)(col)

			if (this._wasm.getValue(ptr, 'i1')) {
				value = this._fromCString(this._wasm.getValue(ptr + 8, 'i8*'))
			} else {
				value = this._wasm.getValue(ptr + 8, 'double')
			}
		} finally {
			this._wasm._free(col)
		}
		return value
	}
}

export class CData extends CObject {
	getMetaInfo(): Mirrored<Data.Metainfo> {
		const cInfo = this._call(this._wasm._data_metaInfo)()
		const info = this._fromCString(cInfo)
		return { series: JSON.parse(info) }
	}

	addDimension(
		name: string,
		indexes: number[],
		categories: string[],
		isContiguous: boolean
	): void {
		const categoriesPointer = new Uint32Array(categories.length)
		for (let i = 0; i < categories.length; i++) {
			categoriesPointer[i] = this._toCString(categories[i]!)
		}

		const categoriesPointerArrayLen = categories.length * 4

		const categoriesPtrArr = this._wasm._malloc(categoriesPointerArrayLen)

		new Uint8Array(this._wasm.HEAPU8.buffer, categoriesPtrArr, categoriesPointerArrayLen).set(
			new Uint8Array(categoriesPointer.buffer)
		)

		const indexesPointerArrayLen = indexes.length * 4

		const indexesArr = this._wasm._malloc(indexesPointerArrayLen)

		new Uint8Array(this._wasm.HEAPU8.buffer, indexesArr, indexesPointerArrayLen).set(
			new Uint8Array(new Uint32Array(indexes).buffer)
		)

		const cname = this._toCString(name)

		try {
			this._call(this._wasm._data_addDimension)(
				cname,
				categoriesPtrArr,
				categories.length,
				indexesArr,
				indexes.length,
				isContiguous
			)
		} finally {
			this._wasm._free(cname)
			for (const categoryPointer of categoriesPointer) {
				this._wasm._free(categoryPointer)
			}
			this._wasm._free(indexesArr)
			this._wasm._free(categoriesPtrArr)
		}
	}

	addMeasure(name: string, unit: string, values: number[]): void {
		const vals = new Float64Array(values)
		const valArrayLen = values.length * 8

		const valArr = this._wasm._malloc(valArrayLen)
		const valHeap = new Uint8Array(this._wasm.HEAPU8.buffer, valArr, valArrayLen)

		valHeap.set(new Uint8Array(vals.buffer))

		const cname = this._toCString(name)
		const cunit = this._toCString(unit)

		try {
			this._call(this._wasm._data_addMeasure)(cname, cunit, valArr, values.length)
		} finally {
			this._wasm._free(cname)
			this._wasm._free(cunit)
			this._wasm._free(valArr)
		}
	}

	addRecord(record: (string | number)[]): void {
		const ptrs = new Uint32Array(record.length)

		for (let i = 0; i < record.length; i++) {
			const ptr = this._toCString(String(record[i]).toString())
			ptrs[i] = ptr
		}

		const ptrArrayLen = record.length * 4

		const ptrArr = this._wasm._malloc(ptrArrayLen)
		const ptrHeap = new Uint8Array(this._wasm.HEAPU8.buffer, ptrArr, ptrArrayLen)
		ptrHeap.set(new Uint8Array(ptrs.buffer))

		try {
			this._call(this._wasm._data_addRecord)(ptrArr, record.length)
		} finally {
			for (const ptr of ptrs) {
				this._wasm._free(ptr)
			}
			this._wasm._free(ptrArr)
		}
	}

	setFilter(callback: ((record: CRecord) => boolean) | null, chartId: CPointer): void {
		const callbackPtrs: [CPointer, CPointer] = [0, 0]
		if (callback !== null) {
			const f = (recordPtr: CRecordPtr): boolean => callback(new CRecord(this, recordPtr))
			callbackPtrs[0] = this._wasm.addFunction(f, 'ii')
			const deleter = (ptr: CPointer): void => {
				if (ptr !== callbackPtrs[0]) console.warn('Wrong pointer passed to destructor')
				this._wasm.removeFunction(callbackPtrs[0])
				this._wasm.removeFunction(callbackPtrs[1])
			}
			callbackPtrs[1] = this._wasm.addFunction(deleter, 'vi')
		}
		this._callStatic(this._wasm._chart_setFilter).bind(this, chartId)(...callbackPtrs)
	}
}
