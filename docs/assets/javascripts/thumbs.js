function setupVideos() {
	const videos = document.querySelectorAll('video.image-gallery')
	videos.forEach((video) => {
		const playPromise = video.play()
		if (playPromise !== undefined) {
			playPromise.then((_) => {
				const observer = new IntersectionObserver(
					(entries) => {
						entries.forEach((entry) => {
							if (entry.intersectionRatio !== 1 && !video.paused) {
								video.pause()
							} else if (video.paused) {
								video.play()
							}
						})
					},
					{ threshold: 0.2 }
				)
				observer.observe(video)
			})
		}
	})
}

const currentScript = document.currentScript
document.addEventListener('DOMContentLoaded', (event) => {
	const parentContainer = currentScript.nextElementSibling
	parentContainer.style.display = 'flex'
	parentContainer.style['flex-wrap'] = 'wrap'
	parentContainer.style.justifyContent = 'center'
	setupVideos()
})
