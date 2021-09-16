"use strict";

export class VideoRecord
{
	constructor(canvasName, callback)
	{
		let recordedChunks = [];
		let canvas = document.getElementById(canvasName);
		let stream = canvas.captureStream(25 /*fps*/);
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

			const link = document.createElement('a');
			link.href = data;

			// this is necessary as link.click() does not work on the latest firefox
			link.dispatchEvent(
				new MouseEvent('click', {
					bubbles: true,
					cancelable: true,
					view: window
				})
			);

			setTimeout(() => {
				// For Firefox it is necessary to delay revoking the ObjectURL
				window.URL.revokeObjectURL(data);
				link.remove();
			}, 100);
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