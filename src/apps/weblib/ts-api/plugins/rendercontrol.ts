import { Plugin, PluginApi, PluginHooks, RenderContext, StartContext } from '../plugins.js'

export interface RenderControlApi extends PluginApi {
	/** Re-renders the chart. */
	update(timeInMSecs: number | null): void
}

export class RenderControl implements Plugin {
	private _update: () => void = () => {}
	private _timeInMSecs: number | null = null
	private _enabled: boolean = true

	meta = { name: 'rendering' }

	get api(): RenderControlApi {
		return {
			update: (timeInMSecs: number | null = null): void => {
				this._timeInMSecs = timeInMSecs
				this._update()
			}
		}
	}

	get hooks(): PluginHooks {
		const hooks = {
			start: (ctx: StartContext, next: () => void): void => {
				this._update = ctx.update
				next()
			},
			render: (ctx: RenderContext, next: () => void): void => {
				if (this._timeInMSecs !== null) {
					ctx.timeInMSecs = this._timeInMSecs
					ctx.force = true
					this._timeInMSecs = null
				}
				ctx.enable = this._enabled
				next()
			}
		}
		return hooks
	}

	enable(enabled: boolean): void {
		this._enabled = enabled
	}
}
