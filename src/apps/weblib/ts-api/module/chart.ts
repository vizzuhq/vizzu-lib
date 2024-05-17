import * as C from '../cvizzu.types.js'

export interface Chart {
	doChange(): void
	openUrl(url: C.CString): void
}
