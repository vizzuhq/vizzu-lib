export class DataRecord {
  constructor(cRecord) {
    return new Proxy(this, {
      get: (_target, columnName) => {
        return cRecord.getValue(columnName)
      }
    })
  }
}
