import { Anim } from './types/anim.js'

import { CAnimControl, CAnimation } from './module/canimctrl.js'

/** Control object for animation. */
export class AnimControl {
  private _cControl: CAnimControl

  constructor(cControl: CAnimControl) {
    this._cControl = cControl
  }

  /** Returns a reference to the actual animation for further reuse. */
  store(): CAnimation {
    return this._cControl.storeAnim()
  }

  /** Seeks the animation to the position specified by time or progress 
    percentage. Seeking the animation to the end position will not trigger
    the {@link Vizzu.animate|animation promise} to resolve.  */
  seek(value: Anim.Position): this {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('seek', param)
    return this
  }

  /** Playback speed of the animation. It is 1.0 by default.
    Negative values are considered 0. */
  // eslint-disable-next-line  accessor-pairs
  set speed(value: number) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('setSpeed', param)
  }

  /** Pauses the controlled animation. */
  pause(): this {
    this._animControl('pause')
    return this
  }

  /** Plays/resumes playing of the controlled animation. */
  play(): this {
    this._animControl('play')
    return this
  }

  /** Stops the current animation seeking it back to its start position. */
  stop(): this {
    this._animControl('stop')
    return this
  }

  /** Cancels the animation, will reject the animation promise. */
  cancel(): this {
    this._animControl('cancel')
    return this
  }

  /** Changes the direction of the controlled animation. */
  reverse(): this {
    this._animControl('reverse')
    return this
  }

  private _animControl(command: string, param = ''): void {
    this._cControl.animControl(command, param)
  }
}
