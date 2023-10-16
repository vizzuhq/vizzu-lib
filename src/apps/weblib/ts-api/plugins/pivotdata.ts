import { Data, Plugins } from '../types/vizzu.js'

import UnPivot from './unpivot.js'

export class PivotData implements Plugins.Plugin {
  meta = {
    name: 'pivotData'
  }

  get hooks(): Plugins.PluginHooks {
    return {
      prepareAnimation: (ctx: Plugins.SetAnimParamsContext, next: () => void): void => {
        if (Array.isArray(ctx.target))
          ctx.target.forEach(({ target }) => {
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

  private static _is1NF(data: Data.Set | Data.Cube): data is Data.Set {
    return 'series' in data || 'records' in data
  }
}
