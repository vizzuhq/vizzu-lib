import '../vizzu.js'
import { AnimCompleting } from '../animcompleting.js'
import { AnyAnimOptions, AnyAnimTarget } from './shorthands.js'

declare module '../vizzu' {
	interface Vizzu {
		animate(target: AnyAnimTarget, options?: AnyAnimOptions): AnimCompleting
	}
}
