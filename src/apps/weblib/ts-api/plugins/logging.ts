import { Plugins } from '../types/vizzu.js'

import Vizzu from '../vizzu.js'
import { NotInitializedError } from '../errors.js'

export class Logging implements Plugins.Plugin {
  private _vizzu?: Vizzu

  meta = { name: 'logging' }

  register(vizzu: Vizzu): void {
    this._vizzu = vizzu
  }

  enable(enabled: boolean): void {
    if (!this._vizzu || !this._vizzu._chart) throw new NotInitializedError()
    this._vizzu._chart._module.setLogging(enabled)
  }
}
