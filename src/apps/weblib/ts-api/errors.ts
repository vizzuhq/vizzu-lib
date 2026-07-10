export class CancelError extends Error {
	constructor() {
		super('animation canceled')
		this.name = 'CancelError'
	}
}

export class NotInitializedError extends Error {
	constructor() {
		super('vizzu is not initialized, use `initializing` promise')
		this.name = 'NotInitializedError'
	}
}

export class VizzuFinalized extends Error {
	constructor() {
		super('vizzu is finalized by detach(), no operation is allowed')
		this.name = 'VizzuFinalized'
	}
}
