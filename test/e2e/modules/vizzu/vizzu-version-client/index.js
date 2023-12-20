function catchError(err) {
	console.error(err)
	let errMsg = err.toString()
	if (err.stack !== undefined) {
		errMsg = err.stack
	}
	window.vizzuVersion = errMsg
	document.title = 'Finished'
}

try {
	const queryString = window.location.search
	const urlParams = new URLSearchParams(queryString)
	const vizzuUrl = urlParams.get('vizzuUrl')

	import(vizzuUrl)
		.then((vizzuModule) => {
			const Vizzu = vizzuModule.default
			const chart = new Vizzu('vizzuCanvas')
			return chart.initializing
		})
		.then((chart) => {
			window.vizzuVersion = chart.version()
			document.title = 'Finished'
		})
		.catch((err) => {
			catchError(err)
		})
} catch (err) {
	catchError(err)
}
