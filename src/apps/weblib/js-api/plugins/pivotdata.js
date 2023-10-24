import { Snapshot } from '../module/cchart.js'
import UnPivot from './unpivot.js'
export class PivotData {
  constructor() {
    this.meta = {
      name: 'pivotData'
    }
  }
  get hooks() {
    return {
      prepareAnimation: (ctx, next) => {
        if (Array.isArray(ctx.target))
          ctx.target.forEach(({ target }) => {
            if (target instanceof Snapshot) return
            if (!target.data) return
            if (target.data && UnPivot.isPivot(target.data)) {
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
    return 'series' in data || 'records' in data
  }
}
