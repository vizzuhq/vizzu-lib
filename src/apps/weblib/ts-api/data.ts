import { Data as D } from './types/vizzu.js'

import { CRecord, CData } from './module/cdata.js'

class DataRecord implements D.Record {
  [seriesName: D.SeriesName]: D.Value

  constructor(cRecord: CRecord) {
    return new Proxy(this, {
      get: (_target, columnName: string) => {
        return cRecord.getValue(columnName)
      }
    })
  }
}

export class Data {
  _cData: CData

  constructor(cData: CData) {
    this._cData = cData
  }

  set(obj?: D.TableBySeries | D.TableByRecords) {
    if (!obj) {
      return
    }

    if (obj.series) {
      if (!Array.isArray(obj.series)) {
        throw new Error('data series field is not an array')
      }

      for (const series of obj.series) {
        this.setSeries(series)
      }
    }

    if ('records' in obj) {
      if (!Array.isArray(obj.records)) {
        throw new Error('data records field is not an array')
      }

      const seriesList = this._cData.getMetaInfo().series
      for (const record of obj.records) {
        this.addRecord(record, seriesList)
      }
    }

    if (obj.filter || obj.filter === null) {
      this.setFilter(obj.filter)
    }
  }

  addRecord(record: D.ValueArray | D.Record, seriesList: D.SeriesInfo[]): void {
    if (!Array.isArray(record)) {
      if (typeof record === 'object' && record !== null) {
        record = this.recordObjectToArray(record, seriesList)
      } else throw new Error('data record is not an array or object')
    }
    this._cData.addRecord(record)
  }

  recordObjectToArray(record: D.Record, seriesList: D.SeriesInfo[]): D.Value[] {
    const result = [] as D.Value[]

    seriesList.forEach((series) => {
      if (series.name in record) {
        result.push(record[series.name] || this._default(series.type))
      } else {
        result.push(this._default(series.type))
      }
    })

    return result
  }

  _default(type?: string) {
    return type === 'measure' ? 0 : ''
  }

  setSeries(series: D.Series): void {
    if (!series.name) {
      throw new Error('missing series name')
    }

    const values = series.values ? series.values : ([] as D.Values)

    const seriesType = series.type ? series.type : this.detectType(values)

    if (seriesType === 'dimension') {
      this.addDimension(series.name, values)
    } else if (seriesType === 'measure') {
      if (!series.unit) series.unit = ''
      this.addMeasure(series.name, series.unit, values)
    } else {
      throw new Error('invalid series type: ' + series.type)
    }
  }

  detectType(values: (string | number)[]): D.SeriesType | null {
    if (Array.isArray(values) && values.length) {
      if (typeof values[0] === 'number') {
        return 'measure'
      } else if (typeof values[0] === 'string') {
        return 'dimension'
      }
    }
    return null
  }

  addDimension(name: string, dimension: unknown[]): void {
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

  _isStringArray(values: any[]): values is string[] {
    for (const value of values) {
      if (typeof value !== 'string') {
        return false
      }
    }
    return true
  }

  addMeasure(name: string, unit: string, values: unknown[]): void {
    if (typeof name !== 'string') {
      throw new Error("'name' parameter should be string")
    }

    if (typeof unit !== 'string') {
      throw new Error("'unit' parameter should be string")
    }

    if (!(values instanceof Array)) {
      throw new Error("'values' parameter should be an array")
    }

    let numbers = values.map((value) => Number(value))

    this._cData.addMeasure(name, unit, numbers)
  }

  setFilter(filter: D.FilterCallback | null): void {
    if (typeof filter === 'function') {
      const callback = (cRecord: CRecord) => filter(new DataRecord(cRecord))
      this._cData.setFilter(callback)
    } else if (filter === null) {
      this._cData.setFilter(null)
    } else {
      throw new Error('data filter is not a function or null')
    }
  }
}
