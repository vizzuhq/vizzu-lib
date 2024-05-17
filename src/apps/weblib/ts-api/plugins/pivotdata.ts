import { Snapshot } from '../module/cchart.js'
import { Plugin, PluginHooks, PrepareAnimationContext } from '../plugins.js'
import * as Data from '../types/data.js'

import UnPivot, { DataCube } from './unpivot.js'

export class PivotData implements Plugin {
	meta = {
		name: 'pivotData'
	}

	get hooks(): PluginHooks {
		return {
			prepareAnimation: (ctx: PrepareAnimationContext, next: () => void): void => {
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

	private static _is1NF(data: Data.Set | DataCube): data is Data.Set {
		return 'series' in data || 'records' in data
	}
}
