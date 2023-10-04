import UnPivot from './unpivot.js'

export default class PivotData {
  meta = {
    name: 'pivotData'
  }

  get hooks() {
    return {
      setAnimParams: (ctx, next) => {
        if (Array.isArray(ctx.target))
          ctx.target.forEach(({ target, options }) => {
            if (target?.data && UnPivot.isPivot(target.data)) {
              if (PivotData._is1NF(target.data)) {
                throw new Error(
                  'inconsistent data form: ' +
                    'series/records and dimensions/measures are both set.'
                )
              } else {
                target.data = UnPivot.convert(target.data)
              }
            }
          })
        next()
      }
    }
  }

  static _is1NF(data) {
    return data.series || data.records
  }
}
