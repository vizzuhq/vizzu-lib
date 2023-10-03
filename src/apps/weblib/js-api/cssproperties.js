import {
  getCSSCustomPropsForElement,
  propsToObject
} from '../../../../../src/apps/weblib/js-api/cssutils.js'

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
      setStyle: (ctx, next) => {
        const props = getCSSCustomPropsForElement(this.chart._container, this.api.prefix)
        ctx.style = propsToObject(props, ctx.style, this.api.prefix)
        next()
      }
    }
  }

  register(chart) {
    this.chart = chart
  }
}
