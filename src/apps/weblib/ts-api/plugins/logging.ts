import { Plugins } from '../types/vizzu.js'

export class Logging implements Plugins.Plugin {
  private _setEnable: (enabled: boolean) => void

  meta = { name: 'logging' }

  constructor(setEnable: (enabled: boolean) => void) {
    this._setEnable = setEnable
  }

  enable(enabled: boolean): void {
    this._setEnable(enabled)
  }
}
