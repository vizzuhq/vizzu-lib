import { Plugin, PluginApi, PluginHooks, RenderContext } from '../plugins.js'

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
			render: (ctx: RenderContext, next: () => void): void => {
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
