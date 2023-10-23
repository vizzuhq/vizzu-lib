import Vizzu from './vizzu'
import { AnimControl } from './animcontrol'

/** Promise resolves to the Vizzu object when the animation completed. */
export interface AnimCompleting extends Promise<Vizzu> {
  /** Promise resolves to the animation controller object when the animation starts. */
  activated?: Promise<AnimControl>
}
