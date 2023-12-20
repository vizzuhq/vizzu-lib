import '../vizzu'
import { AnimCompleting } from '../animcompleting'
import { AnyAnimOptions, AnyAnimTarget } from './shorthands'

declare module '../vizzu' {
	interface Vizzu {
		animate(target: AnyAnimTarget, options?: AnyAnimOptions): AnimCompleting
	}
}
