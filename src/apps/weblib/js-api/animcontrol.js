/** Control object for animation. */
export class AnimControl {
  constructor(cControl) {
    this._cControl = cControl
  }
  /** Returns a reference to the actual animation for further reuse. */
  store() {
    return this._cControl.storeAnim()
  }
  /** Seeks the animation to the position specified by time or progress
      percentage. Seeking the animation to the end position while the animation
      is paused will not trigger the animation complete promise to resolve. */
  seek(value) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('seek', param)
    return this
  }
  /** Playback speed of the animation. It is 1.0 by default.
      Negative values are considered 0. */
  // eslint-disable-next-line  accessor-pairs
  set speed(value) {
    const param = typeof value !== 'string' ? value.toString() : value
    this._animControl('setSpeed', param)
  }
  /** Pauses the controlled animation. */
  pause() {
    this._animControl('pause')
    return this
  }
  /** Plays/resumes playing of the controlled animation. */
  play() {
    this._animControl('play')
    return this
  }
  /** Stops the current animation seeking it back to its start position. */
  stop() {
    this._animControl('stop')
    return this
  }
  /** Cancels the animation, will reject the animation promise. */
  cancel() {
    this._animControl('cancel')
    return this
  }
  /** Changes the direction of the controlled animation. */
  reverse() {
    this._animControl('reverse')
    return this
  }
  _animControl(command, param = '') {
    this._cControl.animControl(command, param)
  }
}
