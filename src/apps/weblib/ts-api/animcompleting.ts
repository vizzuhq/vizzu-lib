import Vizzu from './vizzu.js'
import { AnimControl } from './animcontrol.js'

/** Promise resolves to the Vizzu object when the animation completed. */
export interface AnimCompleting extends Promise<Vizzu> {
	/** Promise resolves to the animation controller object when the animation starts. */
	activated?: Promise<AnimControl>
}
