import { Plugins } from '../types/plugins.js'
import { Snapshot } from '../module/cchart.js'

import Vizzu from '../vizzu.js'

import { getCSSCustomPropsForElement, propsToObject } from './cssutils.js'

export class CSSProperties implements Plugins.Plugin {
  private _chart?: Vizzu

  meta = {
    name: 'cssProperties'
  }

  api = {
    prefix: 'vizzu'
  }

  constructor(options: { prefix?: string } = {}) {
    if (options?.prefix) {
      this.api.prefix = options.prefix
    }
  }

  get hooks(): Plugins.PluginHooks {
    return {
      prepareAnimation: (ctx: Plugins.PrepareAnimationContext, next: () => void): void => {
        const props = getCSSCustomPropsForElement(this._chart!.getCanvasElement(), this.api.prefix)
        if (Array.isArray(ctx.target))
          ctx.target.forEach(({ target }) => {
            if (!(target instanceof Snapshot)) {
              target.style = propsToObject(props, target.style, this.api.prefix)
            }
          })
        next()
      }
    }
  }

  register(chart: Vizzu): void {
    this._chart = chart
  }
}
