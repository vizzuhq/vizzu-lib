import { Plugins } from '../types/vizzu.js'

import Vizzu from '../vizzu.js'

import { getCSSCustomPropsForElement, propsToObject } from './cssutils.js'

export class CSSProperties implements Plugins.Plugin {
  chart?: Vizzu

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

  get hooks() {
    return {
      setAnimParams: (ctx: Plugins.SetAnimParamsContext, next: () => void) => {
        const props = getCSSCustomPropsForElement(this.chart!._container, this.api.prefix)
        if (Array.isArray(ctx.target))
          ctx.target.forEach(({ target }) => {
            target.style = propsToObject(props, target.style, this.api.prefix)
          })
        next()
      }
    }
  }

  register(chart: Vizzu) {
    this.chart = chart
  }
}
