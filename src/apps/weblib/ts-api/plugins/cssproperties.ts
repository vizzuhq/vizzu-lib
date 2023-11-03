import { Plugin, PluginHooks, PrepareAnimationContext } from '../plugins.js'
import { Snapshot } from '../module/cchart.js'

import Vizzu from '../vizzu.js'

import { getCSSCustomPropsForElement, propsToObject } from './cssutils.js'
import { HtmlCanvasApi } from '../htmlcanvas.js'

export class CSSProperties implements Plugin {
  private _chart?: Vizzu

  meta = {
    name: 'cssProperties',
    depends: ['htmlCanvas']
  }

  api = {
    prefix: 'vizzu'
  }

  constructor(options: { prefix?: string } = {}) {
    if (options?.prefix) {
      this.api.prefix = options.prefix
    }
  }

  get hooks(): PluginHooks {
    return {
      prepareAnimation: (ctx: PrepareAnimationContext, next: () => void): void => {
        const props = getCSSCustomPropsForElement(
          (this._chart!.feature['htmlCanvas'] as HtmlCanvasApi).element,
          this.api.prefix
        )
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
