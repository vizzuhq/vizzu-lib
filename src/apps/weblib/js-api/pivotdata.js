import UnPivot from './unpivot.js'

export default class PivotData {
  meta = {
    name: 'pivotData'
  }

  get hooks() {
    return {
      setData: (ctx, next) => {
        if (ctx.data && UnPivot.isPivot(ctx.data)) {
          if (PivotData._is1NF(ctx.data)) {
            throw new Error(
              'inconsistent data form: ' + 'series/records and dimensions/measures are both set.'
            )
          } else {
            ctx.data = UnPivot.convert(ctx.data)
          }
        }
        next()
      }
    }
  }

  static _is1NF(data) {
    return data.series || data.records
  }
}
