export class VideoRecorder {
	constructor(canvasName, callback) {
		const recordedChunks = []
		const canvas = document.getElementById(canvasName)
		const stream = canvas.captureStream(30 /* fps */)
		this.mediaRecorder = new MediaRecorder(stream)

		this.mediaRecorder.ondataavailable = function (e) {
			recordedChunks.push(e.data)
		}

		this.mediaRecorder.onstop = (event) => {
			const blob = new Blob(recordedChunks, {
				type: 'video/webm'
			})
			const data = URL.createObjectURL(blob)
			callback(data)
		}
	}

	start() {
		this.mediaRecorder.start()
	}

	stop() {
		this.mediaRecorder.stop()
	}
}
