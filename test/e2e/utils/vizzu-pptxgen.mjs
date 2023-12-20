export default class PPTXGen {
	static videoCaptureOptions = {
		recorder: { mimeType: 'video/webm;codecs=h264' },
		output: { mimeType: 'video/mp4' }
	}

	meta = {
		name: 'pptxGen',
		version: '0.0.1',
		depends: ['videoCapture']
	}

	get api() {
		return {
			save: (filename) => {
				this.generating = this.generating.then(() => {
					this.presentation.writeFile({ filename })
				})
				return this.generating
			}
		}
	}

	hooks = {
		registerAnimation: (ctx, next) => {
			ctx.promise = ctx.promise.then((chart) => {
				this.chart.feature.videoCapture.start()
				return chart
			})
			next()
			const activated = ctx.promise.activated
			ctx.promise = ctx.promise.then(this._addSlide.bind(this))
			ctx.promise.activated = activated
		}
	}

	constructor(options) {
		this.options = options
		this.presentation = new PptxGenJS() // eslint-disable-line no-undef
		this.generating = Promise.resolve()
	}

	register(chart) {
		this.chart = chart
	}

	async _addSlide() {
		const capture = await this.chart.feature.videoCapture.stop()
		const base64String = await this._blobToBase64(capture.blob)
		const slide = this.presentation.addSlide()
		slide.addMedia({ type: 'video', data: base64String, w: '100%', h: '100%' })
	}

	_blobToBase64(blob) {
		return new Promise((resolve) => {
			const reader = new FileReader()
			reader.onloadend = function () {
				resolve(reader.result)
			}
			reader.readAsDataURL(blob)
		})
	}
}
