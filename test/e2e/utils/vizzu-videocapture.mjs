export default class VideoCapture {
	meta = {
		name: 'videoCapture',
		version: '0.0.1',
		depends: []
	}

	get api() {
		return {
			start: () => {
				this.mediaRecorder.start()
			},
			stop: () => {
				const completed = new Promise((resolve) => {
					this._rendered = resolve
				})
				this.mediaRecorder.stop()
				return completed
			}
		}
	}

	constructor(options) {
		this.options = {
			stream: { frameRate: 30, ...options?.stream },
			recorder: { mimeType: 'video/webm', ...options?.recorder },
			output: {
				mimeType: options?.output?.mimeType || options?.recorder?.mimeType || 'video/webm',
				...options?.output
			}
		}
	}

	register(chart) {
		this.chart = chart
		this._init()
	}

	_init() {
		const recordedChunks = []
		const stream = this.chart.feature.htmlCanvas.element.captureStream(
			this.options.stream.frameRate
		)
		this.mediaRecorder = new MediaRecorder(stream, this.options.recorder)

		this.mediaRecorder.ondataavailable = (e) => {
			recordedChunks.push(e.data)
		}

		this.mediaRecorder.onstop = (event) => {
			const blob = new Blob(recordedChunks, {
				type: this.options.output.mimeType
			})
			this._init()
			this._rendered({ blob, getObjectURL: () => URL.createObjectURL(blob) })
		}
	}
}
