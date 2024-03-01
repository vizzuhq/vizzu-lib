import { Plugin, PluginHooks, StartContext } from '../plugins.js'

export class Scheduler implements Plugin {
	private _updateInterval?: ReturnType<typeof setInterval>
	private _update: () => void = () => {}
	private _enabled = false

	meta = { name: 'scheduler' }

	get hooks(): PluginHooks {
		const hooks = {
			start: (ctx: StartContext, next: () => void): void => {
				this._update = ctx.update
				this._start()
				next()
			}
		}
		return hooks
	}

	enable(enabled: boolean): void {
		this._enabled = enabled
		if (this._enabled) this._start()
		else this._stop()
	}

	unregister(): void {
		this._stop()
	}

	private _start(): void {
		if (!this._updateInterval) {
			this._update()
			this._updateInterval = setInterval(() => {
				if (this._enabled) {
					this._update()
				}
			}, 25)
		}
	}

	private _stop(): void {
		if (this._updateInterval) clearInterval(this._updateInterval)
		delete this._updateInterval
	}
}
