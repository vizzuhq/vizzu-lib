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
  let queryString = window.location.search
  let urlParams = new URLSearchParams(queryString)
  let vizzuUrl = urlParams.get('vizzuUrl')

  import(vizzuUrl)
    .then((vizzuModule) => {
      let Vizzu = vizzuModule.default
      let chart = new Vizzu('vizzuCanvas')
      return chart.initializing
    })
    .then((chart) => {
      window.vizzuVersion = chart.version()
      document.title = 'Finished'
      return
    })
    .catch((err) => {
      catchError(err)
    })
} catch (err) {
  catchError(err)
}
