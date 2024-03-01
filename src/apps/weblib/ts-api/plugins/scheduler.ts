import { Plugin, PluginHooks, StartSchedulerContext } from '../plugins.js'

export class Scheduler implements Plugin {
	private _updateInterval?: ReturnType<typeof setInterval>
	private _update: (time: number) => void = () => {}
	private _enabled = false

	meta = { name: 'scheduler' }

	get hooks(): PluginHooks {
		const hooks = {
			startScheduler: (ctx: StartSchedulerContext, next: () => void): void => {
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
			this._update(performance.now())
			this._updateInterval = setInterval(() => {
				if (this._enabled) {
					this._update(performance.now())
				}
			}, 25)
		}
	}

	private _stop(): void {
		if (this._updateInterval) clearInterval(this._updateInterval)
		delete this._updateInterval
	}
}
