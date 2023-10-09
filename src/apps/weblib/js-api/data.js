class DataRecord {
  constructor(cRecord) {
    this.cRecord = cRecord

    return new Proxy(this, {
      get: (target, columnName) => {
        return target.cRecord.getValue(columnName)
      }
    })
  }
}

export class Data {
  constructor(chart) {
    this.chart = chart
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
    this.chart._cData.addRecord(record)
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

    for (let i = 0; i < dimension.length; i++) {
      if (typeof dimension[i] !== 'string' && !(dimension[i] instanceof String)) {
        throw new Error('array element should be string')
      }
    }

    this.chart._cData.addDimension(name, dimension)
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

    this.chart._cData.addMeasure(name, unit, values)
  }

  setFilter(filter) {
    if (typeof filter === 'function') {
      const callback = (cRecord) => filter(new DataRecord(cRecord))
      this.chart._cData.setFilter(callback)
    } else if (filter === null) {
      this.chart._cData.setFilter(null)
    } else {
      throw new Error('data filter is not a function or null')
    }
  }
}
