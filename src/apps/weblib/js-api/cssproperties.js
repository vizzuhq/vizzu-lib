import { getCSSCustomPropsForElement, propsToObject } from './cssutils.js'

export default class CSSProperties {
  meta = {
    name: 'cssProperties'
  }

  api = {
    prefix: 'vizzu'
  }

  constructor(options) {
    if (options?.prefix) {
      this.api.prefix = options.prefix
    }
  }

  get hooks() {
    return {
      setAnimParams: (ctx, next) => {
        const props = getCSSCustomPropsForElement(this.chart._container, this.api.prefix)
        if (Array.isArray(ctx.target))
          ctx.target.forEach(({ target, options }) => {
            target.style = propsToObject(props, target.style, this.api.prefix)
          })
        next()
      }
    }
  }

  register(chart) {
    this.chart = chart
  }
}
