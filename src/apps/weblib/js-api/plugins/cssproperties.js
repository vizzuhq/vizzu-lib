import { Snapshot } from '../module/cchart.js'
import { getCSSCustomPropsForElement, propsToObject } from './cssutils.js'
export class CSSProperties {
  constructor(options = {}) {
    this.meta = {
      name: 'cssProperties'
    }
    this.api = {
      prefix: 'vizzu'
    }
    if (options?.prefix) {
      this.api.prefix = options.prefix
    }
  }
  get hooks() {
    return {
      prepareAnimation: (ctx, next) => {
        const props = getCSSCustomPropsForElement(this._chart.getCanvasElement(), this.api.prefix)
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
  register(chart) {
    this._chart = chart
  }
}
