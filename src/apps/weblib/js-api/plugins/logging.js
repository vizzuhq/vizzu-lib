export class Logging {
  constructor(setEnable) {
    this.meta = { name: 'logging' }
    this._setEnable = setEnable
  }
  enable(enabled) {
    this._setEnable(enabled)
  }
}
