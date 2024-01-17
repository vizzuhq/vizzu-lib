import { Position } from './types/anim.js'

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
    percentage. Seeking the animation to the end position while the animation
    is paused will not trigger the animation complete promise to resolve. */
	seek(value: Position): this {
		const param = typeof value !== 'string' ? value.toString() : value
		this._setParam('seek', param)
		return this
	}

	/** Setting the playback speed of the animation. It is 1.0 by default.
    Negative values are considered 0. */
	set speed(value: number) {
		const param = typeof value !== 'string' ? value.toString() : value
		this._setParam('speed', param)
	}

	/** Getting the playback speed of the animation. */
	get speed(): number {
		return Number(this._getParam('speed'))
	}

	/** Setting the playback state of the animation. */
	set playState(value: 'running' | 'paused') {
		this._setParam('playState', value)
	}

	/** Getting the playback state of the animation. */
	get playState(): 'running' | 'paused' {
		return this._getParam('playState') as 'running' | 'paused'
	}

	/** Setting the play direction of the animation. */
	set direction(value: 'normal' | 'reverse') {
		this._setParam('direction', value)
	}

	/** Getting the play direction of the animation. */
	get direction(): 'normal' | 'reverse' {
		return this._getParam('direction') as 'normal' | 'reverse'
	}

	/** Setting the position of the animation. */
	set position(value: number) {
		const param = typeof value !== 'string' ? value.toString() : value
		this._setParam('position', param)
	}

	/** Getting the position of the animation. */
	get position(): number {
		return this._getParam('position') as number
	}

	/** @deprecated Pauses the controlled animation. */
	pause(): this {
		this.playState = 'paused'
		return this
	}

	/** @deprecated Plays/resumes playing of the controlled animation. */
	play(): this {
		this.playState = 'running'
		return this
	}

	/** Stops the current animation seeking it back to its start position. */
	stop(): this {
		this._setParam('stop')
		return this
	}

	/** Cancels the animation, will reject the animation promise. */
	cancel(): this {
		this._setParam('cancel')
		return this
	}

	/** @deprecated Changes the direction of the controlled animation to reverse. */
	reverse(): this {
		this.direction = this.direction === 'reverse' ? 'normal' : 'reverse'
		return this
	}

	private _setParam(command: string, param = ''): void {
		this._cControl.setParam(command, param)
	}

	private _getParam(command: string): unknown {
		return this._cControl.getParam(command)
	}
}
