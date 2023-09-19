import UnPivot from './unpivot.js'

class DataRecord {
  constructor(chart, record) {
    this.chart = chart
    this.record = record

    return new Proxy(this, {
      get: (target, columnName) => {
        return target._getValue(columnName)
      }
    })
  }

  _getValue(columnName) {
    const col = this.chart._toCString(columnName)
    let ptr
    let value

    try {
      ptr = this.chart._call(this.chart.module._record_getValue)(this.record, col, true)

      if (this.chart.module.getValue(ptr, 'i1')) {
        value = this.chart._fromCString(this.chart.module.getValue(ptr + 8, 'i8*'))
      } else {
        value = this.chart.module.getValue(ptr + 8, 'double')
      }
    } finally {
      this.chart.module._free(col)
    }
    return value
  }
}

export default class Data {
  constructor(chart) {
    this.chart = chart
  }

  set(obj) {
    if (!obj) {
      return
    }

    if (UnPivot.isPivot(obj)) {
      if (this.is1NF(obj)) {
        throw new Error(
          'inconsistent data form: ' + 'series/records and dimensions/measures are both set.'
        )
      } else {
        obj = UnPivot.convert(obj)
      }
    }

    if (obj.series) {
      if (!Array.isArray(obj.series)) {
        throw new Error('data series field is not an array')
      }

      for (const series of obj.series) {
        this.setSeries(series)
      }
    }

    if (obj.records) {
      if (!Array.isArray(obj.records)) {
        throw new Error('data records field is not an array')
      }

      const seriesList = this.chart.data.series
      for (const record of obj.records) {
        this.addRecord(record, seriesList)
      }
    }

    if (obj.filter || obj.filter === null) {
      this.setFilter(obj.filter)
    }
  }

  addRecord(record, seriesList) {
    if (!Array.isArray(record)) {
      if (typeof record === 'object' && record !== null) {
        record = this.recordObjectToArray(record, seriesList)
      } else throw new Error('data record is not an array or object')
    }

    const ptrs = new Uint32Array(record.length)
    for (let i = 0; i < record.length; i++) {
      const ptr = this.chart._toCString(String(record[i]).toString())
      ptrs[i] = ptr
    }

    const ptrArrayLen = record.length * 4

    const ptrArr = this.chart.module._malloc(ptrArrayLen)
    const ptrHeap = new Uint8Array(this.chart.module.HEAPU8.buffer, ptrArr, ptrArrayLen)
    ptrHeap.set(new Uint8Array(ptrs.buffer))

    try {
      this.chart._call(this.chart.module._data_addRecord)(ptrArr, record.length)
    } finally {
      for (const ptr of ptrs) {
        this.chart.module._free(ptr)
      }
      this.chart.module._free(ptrArr)
    }
  }

  recordObjectToArray(record, seriesList) {
    const result = []

    seriesList.forEach((series) => {
      if (series.name in record) {
        result.push(record[series.name])
      } else {
        result.push(series.type === 'measure' ? 0 : '')
      }
    })

    return result
  }

  setSeries(series) {
    if (!series.name) {
      throw new Error('missing series name')
    }

    if (!series.values) {
      series.values = []
    }

    if (!series.type) {
      series.type = this.detectType(series.values)
    }

    if (series.type === 'dimension') {
      this.addDimension(series.name, series.values)
    } else if (series.type === 'measure') {
      if (!series.unit) series.unit = ''
      this.addMeasure(series.name, series.unit, series.values)
    } else {
      throw new Error('invalid series type: ' + series.type)
    }
  }

  detectType(values) {
    if (Array.isArray(values) && values.length) {
      if (typeof values[0] === 'number') {
        // number
        return 'measure'
      } else if (values[0] === '' + values[0]) {
        // string
        return 'dimension'
      }
    }

    return null
  }

  addDimension(name, dimension) {
    if (typeof name !== 'string' && !(name instanceof String)) {
      throw new Error('first parameter should be string')
    }

    if (!(dimension instanceof Array)) {
      throw new Error('second parameter should be an array')
    }

    const ptrs = new Uint32Array(dimension.length)
    for (let i = 0; i < dimension.length; i++) {
      if (typeof dimension[i] !== 'string' && !(dimension[i] instanceof String)) {
        throw new Error('array element should be string')
      }

      const ptr = this.chart._toCString(dimension[i])
      ptrs[i] = ptr
    }

    const ptrArrayLen = dimension.length * 4

    const ptrArr = this.chart.module._malloc(ptrArrayLen)
    const ptrHeap = new Uint8Array(this.chart.module.HEAPU8.buffer, ptrArr, ptrArrayLen)
    ptrHeap.set(new Uint8Array(ptrs.buffer))

    const cname = this.chart._toCString(name)

    try {
      this.chart._call(this.chart.module._data_addDimension)(cname, ptrArr, dimension.length)
    } finally {
      this.chart.module._free(cname)
      for (const ptr of ptrs) {
        this.chart.module._free(ptr)
      }
      this.chart.module._free(ptrArr)
    }
  }

  addMeasure(name, unit, values) {
    if (typeof name !== 'string' && !(name instanceof String)) {
      throw new Error("'name' parameter should be string")
    }

    if (typeof unit !== 'string' && !(unit instanceof String)) {
      throw new Error("'unit' parameter should be string")
    }

    if (!(values instanceof Array)) {
      throw new Error("'values' parameter should be an array")
    }

    const vals = new Float64Array(values)
    const valArrayLen = values.length * 8

    const valArr = this.chart.module._malloc(valArrayLen)
    const valHeap = new Uint8Array(this.chart.module.HEAPU8.buffer, valArr, valArrayLen)

    valHeap.set(new Uint8Array(vals.buffer))

    const cname = this.chart._toCString(name)
    const cunit = this.chart._toCString(unit)

    try {
      this.chart._call(this.chart.module._data_addMeasure)(cname, cunit, valArr, values.length)
    } finally {
      this.chart.module._free(cname)
      this.chart.module._free(cunit)
      this.chart.module._free(valArr)
    }
  }

  setFilter(filter) {
    if (typeof filter === 'function') {
      const callback = (ptr) => filter(new DataRecord(this.chart, ptr))
      const callbackPtrs = [this.chart.module.addFunction(callback, 'ii')]
      const deleter = (ptr) => {
        if (ptr !== callbackPtrs[0]) console.warn('Wrong pointer passed to destructor')
        this.chart.module.removeFunction(callbackPtrs[0])
        this.chart.module.removeFunction(callbackPtrs[1])
      }
      callbackPtrs.push(this.chart.module.addFunction(deleter, 'vi'))
      this.chart._call(this.chart.module._chart_setFilter)(...callbackPtrs)
    } else if (filter === null) {
      this.chart._call(this.chart.module._chart_setFilter)(0, 0)
    } else {
      throw new Error('data filter is not a function or null')
    }
  }

  is1NF(data) {
    return data.series || data.records
  }
}
