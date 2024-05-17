export class LastAnimation {
	constructor() {
		this._last = Promise.resolve()
	}

	get api() {
		return {
			last: () => {
				return this._last
			},
			reset: () => {
				this._last = Promise.resolve()
			}
		}
	}

	get hooks() {
		return {
			registerAnimation: (ctx, next) => {
				next()
				this._last = ctx.promise
			}
		}
	}
}
