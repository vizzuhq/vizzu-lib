const iframe = document.getElementById('showcase')

iframe.addEventListener('load', function () {
	const childBody = iframe.contentWindow.document.body

	function resizeIframe() {
		const childHeight = childBody.scrollHeight
		iframe.style.height = childHeight + 50 + 'px' // todo: get showcase explorers size correctly
	}

	resizeIframe()

	iframe.contentWindow.addEventListener('resize', resizeIframe)
})
