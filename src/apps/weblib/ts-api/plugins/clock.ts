import { Plugin, PluginApi, PluginHooks, UpdateContext } from '../plugins.js'

export interface ClockApi extends PluginApi {
	/** Returns the actual time in miliseconds. */
	now(): number
}

export class Clock implements Plugin {
	meta = { name: 'clock' }

	get api(): ClockApi {
		return {
			now: (): number => this._now()
		}
	}

	get hooks(): PluginHooks {
		const hooks = {
			update: (ctx: UpdateContext, next: () => void): void => {
				if (ctx.timeInMSecs === null) ctx.timeInMSecs = this._now()
				next()
			}
		}
		return hooks
	}

	private _now(): number {
		return performance.now()
	}
}
