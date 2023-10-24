import { DataRecord } from './datarecord.js'
export class Data {
  constructor(cData) {
    this._cData = cData
  }
  set(obj) {
    if (!obj) {
      return
    }
    if (obj.series) {
      if (!Array.isArray(obj.series)) {
        throw new Error('data series field is not an array')
      }
      for (const series of obj.series) {
        this._setSeries(series)
      }
    }
    if ('records' in obj) {
      if (!Array.isArray(obj.records)) {
        throw new Error('data records field is not an array')
      }
      const seriesList = this._cData.getMetaInfo().series
      for (const record of obj.records) {
        this._addRecord(record, seriesList)
      }
    }
    if (obj.filter || obj.filter === null) {
      this._setFilter(obj.filter)
    }
  }
  _addRecord(record, seriesList) {
    if (!Array.isArray(record)) {
      if (typeof record === 'object' && record !== null) {
        record = this._recordObjectToArray(record, seriesList)
      } else throw new Error('data record is not an array or object')
    }
    this._cData.addRecord(record)
  }
  _recordObjectToArray(record, seriesList) {
    const result = []
    seriesList.forEach((series) => {
      if (series.name in record) {
        result.push(record[series.name] || this._default(series.type))
      } else {
        result.push(this._default(series.type))
      }
    })
    return result
  }
  _default(type) {
    return type === 'measure' ? 0 : ''
  }
  _setSeries(series) {
    if (!series.name) {
      throw new Error('missing series name')
    }
    const values = series.values ? series.values : []
    const seriesType = series.type ? series.type : this._detectType(values)
    if (seriesType === 'dimension') {
      this._addDimension(series.name, values)
    } else if (seriesType === 'measure') {
      if (!series.unit) series.unit = ''
      this._addMeasure(series.name, series.unit, values)
    } else {
      throw new Error('invalid series type: ' + series.type)
    }
  }
  _detectType(values) {
    if (Array.isArray(values) && values.length) {
      if (typeof values[0] === 'number') {
        return 'measure'
      } else if (typeof values[0] === 'string') {
        return 'dimension'
      }
    }
    return null
  }
  _addDimension(name, dimension) {
    if (typeof name !== 'string') {
      throw new Error('first parameter should be string')
    }
    if (!(dimension instanceof Array)) {
      throw new Error('second parameter should be an array')
    }
    if (!this._isStringArray(dimension)) {
      throw new Error('array element should be string')
    }
    this._cData.addDimension(name, dimension)
  }
  _isStringArray(values) {
    for (const value of values) {
      if (typeof value !== 'string') {
        return false
      }
    }
    return true
  }
  _addMeasure(name, unit, values) {
    if (typeof name !== 'string') {
      throw new Error("'name' parameter should be string")
    }
    if (typeof unit !== 'string') {
      throw new Error("'unit' parameter should be string")
    }
    if (!(values instanceof Array)) {
      throw new Error("'values' parameter should be an array")
    }
    const numbers = values.map((value) => Number(value))
    this._cData.addMeasure(name, unit, numbers)
  }
  _setFilter(filter) {
    if (typeof filter === 'function') {
      const callback = (cRecord) => filter(new DataRecord(cRecord))
      this._cData.setFilter(callback)
    } else if (filter === null) {
      this._cData.setFilter(null)
    } else {
      throw new Error('data filter is not a function or null')
    }
  }
}
