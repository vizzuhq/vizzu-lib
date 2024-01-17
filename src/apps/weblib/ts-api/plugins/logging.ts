import { Plugin } from '../plugins.js'

export class Logging implements Plugin {
	private _setEnable: (enabled: boolean) => void

	meta = { name: 'logging' }

	constructor(setEnable: (enabled: boolean) => void) {
		this._setEnable = setEnable
	}

	enable(enabled: boolean): void {
		this._setEnable(enabled)
	}
}
