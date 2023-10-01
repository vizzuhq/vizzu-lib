export default class Shorthands {
  meta = {
    name: 'shorthands'
  }

  get hooks() {
    const hooks = {
      setConfig: (ctx, next) => {
        this._prepareConfig(ctx)
        next()
      },
      setStyle: (ctx, next) => {
        this._prepareStyle(ctx)
        next()
      },
      setAnimOptions: (ctx, next) => {
        this._prepareAnimOptions(ctx)
        next()
      }
    }
    hooks.setConfig.priority = 1
    hooks.setStyle.priority = 1
    hooks.setAnimOptions.priority = 1
    return hooks
  }

  register(chart) {
    this.chart = chart
    this._channelNames = Object.keys(this.chart.config.channels)
    console.log(this._channelNames)
  }

  _prepareConfig(ctx) {
    const config = ctx.config
    if (config !== null && typeof config === 'object') {
      Object.keys(config).forEach((key) => {
        if (this._channelNames.includes(key)) {
          config.channels = config.channels || {}
          config.channels[key] = config[key]
          delete config[key]
        }
      })
    }

    if (config?.channels) {
      const channels = config.channels
      Object.keys(channels).forEach((ch) => {
        if (typeof channels[ch] === 'string') {
          channels[ch] = [channels[ch]]
        }

        if (channels[ch] === null || Array.isArray(channels[ch])) {
          channels[ch] = { set: channels[ch] }
        }

        if (typeof channels[ch].attach === 'string') {
          channels[ch].attach = [channels[ch].attach]
        }

        if (typeof channels[ch].detach === 'string') {
          channels[ch].detach = [channels[ch].detach]
        }

        if (typeof channels[ch].set === 'string') {
          channels[ch].set = [channels[ch].set]
        }

        if (Array.isArray(channels[ch].set) && channels[ch].set.length === 0) {
          channels[ch].set = null
        }
      })
    }
  }

  _prepareStyle(ctx) {}

  _prepareAnimOptions(ctx) {
    if (typeof ctx.animOptions !== 'undefined') {
      if (ctx.animOptions === null) {
        ctx.animOptions = { duration: 0 }
      } else if (typeof ctx.animOptions === 'string' || typeof ctx.animOptions === 'number') {
        ctx.animOptions = { duration: ctx.animOptions }
      }
    }
  }
}
