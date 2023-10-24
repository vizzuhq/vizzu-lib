import { CObject } from './cenv.js'
export class CRecord extends CObject {
  constructor(env, recordPtr) {
    super(() => recordPtr, env)
  }
  getValue(columnName) {
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
  getMetaInfo() {
    const cInfo = this._call(this._wasm._data_metaInfo)()
    const info = this._fromCString(cInfo)
    return { series: JSON.parse(info) }
  }
  addDimension(name, dimension) {
    const ptrs = new Uint32Array(dimension.length)
    for (let i = 0; i < dimension.length; i++) {
      const ptr = this._toCString(dimension[i])
      ptrs[i] = ptr
    }
    const ptrArrayLen = dimension.length * 4
    const ptrArr = this._wasm._malloc(ptrArrayLen)
    const ptrHeap = new Uint8Array(this._wasm.HEAPU8.buffer, ptrArr, ptrArrayLen)
    ptrHeap.set(new Uint8Array(ptrs.buffer))
    const cname = this._toCString(name)
    try {
      this._call(this._wasm._data_addDimension)(cname, ptrArr, dimension.length)
    } finally {
      this._wasm._free(cname)
      for (const ptr of ptrs) {
        this._wasm._free(ptr)
      }
      this._wasm._free(ptrArr)
    }
  }
  addMeasure(name, unit, values) {
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
  addRecord(record) {
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
  setFilter(callback) {
    const callbackPtrs = [0, 0]
    if (callback !== null) {
      const f = (recordPtr) => callback(new CRecord(this, recordPtr))
      callbackPtrs[0] = this._wasm.addFunction(f, 'ii')
      const deleter = (ptr) => {
        if (ptr !== callbackPtrs[0]) console.warn('Wrong pointer passed to destructor')
        this._wasm.removeFunction(callbackPtrs[0])
        this._wasm.removeFunction(callbackPtrs[1])
      }
      callbackPtrs[1] = this._wasm.addFunction(deleter, 'vi')
    }
    this._call(this._wasm._chart_setFilter)(...callbackPtrs)
  }
}
