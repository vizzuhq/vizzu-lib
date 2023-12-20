export default class MarkerDropshadow {
	defaultStyle = {
		color: '#00000060',
		blur: 9,
		offsetX: 3,
		offsetY: 3
	}

	constructor() {
		this.style = null
		this.nextStyle = null
		this.progress = 0
	}

	meta = {
		name: 'markerDropshadow',
		version: '0.0.1',
		depends: []
	}

	get listeners() {
		return {
			update: (event) => {
				this.progress = event.detail.progress
			},
			'plot-marker-draw': this._setDropshadow.bind(this),
			'plot-marker-label-draw': this._setDropshadow.bind(this)
		}
	}

	hooks = {
		prepareAnimation: (ctx, next) => {
			if (Array.isArray(ctx.target))
				ctx.target.forEach(({ target, options }) => {
					target.style = this._prepareStyle(target.style)
				})
			next()
		}
	}

	_prepareStyle(style) {
		const markerStyle = style?.plot?.marker
		if (!markerStyle) return

		this.nextStyle = {
			...this.defaultStyle,
			...this.style,
			...this.nextStyle
		}

		if (markerStyle.shadowColor) this.nextStyle.color = markerStyle.shadowColor
		if (markerStyle.shadowBlur) this.nextStyle.blur = markerStyle.shadowBlur
		if (markerStyle.shadowOffsetX) this.nextStyle.offsetX = markerStyle.shadowOffsetX
		if (markerStyle.shadowOffsetY) this.nextStyle.offsetY = markerStyle.shadowOffsetY

		this.style = { ...this.nextStyle, ...this.style }

		delete markerStyle.shadowColor
		delete markerStyle.shadowBlur
		delete markerStyle.shadowOffsetX
		delete markerStyle.shadowOffsetY

		return style
	}

	_setDropshadow(event) {
		const ctx = event.renderingContext
		const style = this._actStyle()
		ctx.shadowColor = style.color
		ctx.shadowBlur = style.blur
		ctx.shadowOffsetX = style.offsetX
		ctx.shadowOffsetY = style.offsetY
	}

	_actStyle() {
		const result = {}
		for (const key in this.style) {
			result[key] = this.nextStyle
				? key === 'color'
					? tinycolor
							.mix(this.style[key], this.nextStyle[key], (1 - this.progress) * 100)
							.toRgbString()
					: this.style[key] + (this.nextStyle[key] - this.style[key]) * this.progress
				: this.style[key]
		}
		return result
	}
}
