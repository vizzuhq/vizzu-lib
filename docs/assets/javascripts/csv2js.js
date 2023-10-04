import * as d3 from 'https://cdn.jsdelivr.net/npm/d3@7/+esm'

class Csv2Js {
  static csv(csv, dimensions, measures) {
    return new Promise((resolve, reject) => {
      if (!dimensions) {
        dimensions = []
      }
      if (!measures) {
        measures = []
      }
      const detectedDimensions = {}
      const data = { series: [], records: [] }
      const csvLoaded = d3.csv(csv)

      csvLoaded.then((csvData) => {
        for (let i = 0; i < csvData.length; i++) {
          const record = []
          const keys = Object.keys(csvData[i])
          for (const key of keys) {
            const numValue = +csvData[i][key]
            if (csvData[i][key] !== '' && !isNaN(numValue)) {
              record.push(numValue)
            } else {
              record.push(csvData[i][key])
              detectedDimensions[key] = true
            }
          }
          data.records.push(record)
        }
        for (let i = 0; i < csvData.columns.length; i++) {
          const key = csvData.columns[i]
          const series = {
            name: key,
            type:
              dimensions.includes(key) || (detectedDimensions[key] && !measures.includes(key))
                ? 'dimension'
                : 'measure'
          }
          data.series.push(series)
        }
        return resolve(data)
      })
    })
  }
}

export default Csv2Js
