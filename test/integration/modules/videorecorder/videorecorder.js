"use strict";

export class VideoRecorder
{
	constructor(canvasName, callback)
	{
		let recordedChunks = [];
		let canvas = document.getElementById(canvasName);
		let stream = canvas.captureStream(30 /*fps*/);
		this.mediaRecorder = new MediaRecorder(stream);

		this.mediaRecorder.ondataavailable = function (e) {
			recordedChunks.push(e.data);
		};

		this.mediaRecorder.onstop = event => {
			let blob = new Blob(recordedChunks, {
				type: "video/webm"
			});
			let data = URL.createObjectURL(blob);
			callback(data);
		}
	}

	start()
	{
		this.mediaRecorder.start();
	}

	stop()
	{
		this.mediaRecorder.stop();
	}
}