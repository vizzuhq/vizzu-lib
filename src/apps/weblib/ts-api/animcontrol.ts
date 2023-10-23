import { Anim } from './types/anim.js'

import { CAnimControl, CAnimation } from './module/canimctrl.js'

export class AnimControl implements Anim.Control {
  private _cControl: CAnimControl

  constructor(cControl: CAnimControl) {
    this._cControl = cControl
  }

  store(): CAnimation {
    return this._cControl.storeAnim()
  }

  seek(value: Anim.Position): this {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('seek', param)
    return this
  }

  // eslint-disable-next-line  accessor-pairs
  set speed(value: number) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('setSpeed', param)
  }

  pause(): this {
    this._animControl('pause')
    return this
  }

  play(): this {
    this._animControl('play')
    return this
  }

  stop(): this {
    this._animControl('stop')
    return this
  }

  cancel(): this {
    this._animControl('cancel')
    return this
  }

  reverse(): this {
    this._animControl('reverse')
    return this
  }

  private _animControl(command: string, param = ''): void {
    this._cControl.animControl(command, param)
  }
}
