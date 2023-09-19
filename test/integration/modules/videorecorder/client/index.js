import { VideoRecorder } from './videorecorder.js'

function catchError(err) {
  console.error(err)
  let errMsg = err.toString()
  if (err.stack !== undefined) {
    errMsg = err.stack
  }
  window.result = { result: 'ERROR', description: errMsg }
  document.title = 'Finished'
}

try {
  const queryString = window.location.search
  const urlParams = new URLSearchParams(queryString)
  const testSuitePath = urlParams.get('testSuitePath')
  const testCasesPath = urlParams.get('testCasesPath')
  const testCase = urlParams.get('testCase')
  const testName = urlParams.get('testName')
  const testType = urlParams.get('testType')
  const testIndex = urlParams.get('testIndex')
  const vizzuUrl = urlParams.get('vizzuUrl')
  const reverse = urlParams.get('reverse')

  import(vizzuUrl)
    .then((vizzuModule) => {
      const Vizzu = vizzuModule.default
      return import(testSuitePath + '/' + testCasesPath + '/' + testCase + '.mjs').then(
        (testCasesModule) => {
          const chart = new Vizzu('vizzuCanvas')
          const videoRecorder = new VideoRecorder('vizzuCanvas', (data) => {
            const a = document.createElement('a')
            a.setAttribute('href', data)
            a.setAttribute(
              'download',
              testCasesPath.replaceAll('/', '___') + '___' + testName + '.webm'
            )
            a.click()
            window.result = { result: 'OK' }
            document.title = 'Finished'
          })
          let testSteps = []
          if (testType === 'single') {
            testSteps = testCasesModule.default
          } else if (testType === 'multi') {
            testSteps = testCasesModule.default[testIndex].testSteps
          }
          const steps = []
          const snapshots = []
          let snapshotCnt = 0
          steps.push((chart) => {
            if (reverse === 'true') {
              chart._setAnimation(null)
            } else {
              videoRecorder.start()
            }
            return testSteps[0](chart)
          })
          steps.push((chart) => {
            chart.render.updateFrame(true)
            if (reverse === 'true') {
              videoRecorder.start()
            }
            return chart
          })
          for (let i = 1; i < testSteps.length; i++) {
            snapshotCnt++
            steps.push((chart) => {
              snapshots.push(chart.store())
              return testSteps[i](chart)
            })
          }
          if (reverse === 'true') {
            for (let i = 0; i < snapshotCnt; i++) {
              steps.push((chart) => {
                const res = chart.animate(snapshots.at(-1))
                snapshots.pop()
                return res
              })
            }
          } else {
            steps.push((chart) => {
              chart.on('update', (event) => {
                window.progress = event.detail.progress
              })
              chart.on('background-draw', (event) => {
                let progress = 0
                if (window.progress) {
                  progress = window.progress
                }
                event.renderingContext.fillStyle = '#ffffff'
                const canvas = document.getElementById('vizzuCanvas')
                event.renderingContext.fillRect(-1, -1, canvas.width + 1, canvas.height + 1)
                event.renderingContext.globalAlpha = 1 - progress
              })
              chart.on('logo-draw', (event) => {
                event.renderingContext.globalAlpha = 1
              })
              return chart
            })
            steps.push((chart) => {
              return chart.animate(
                { style: { logo: { filter: 'color(#ffffff00)' } } },
                { duration: '500ms' }
              )
            })
          }

          let promise = chart.initializing
          for (const step of steps) {
            promise = promise.then((chart) => step(chart))
          }
          return promise.then((chart) => {
            setInterval(() => {
              chart.render.updateFrame(true)
            }, 50)
            if (reverse === 'true') {
              setTimeout(() => {
                videoRecorder.stop()
              }, 2000)
            } else {
              videoRecorder.stop()
            }
          })
        }
      )
    })
    .catch((err) => {
      catchError(err)
    })
} catch (err) {
  catchError(err)
}
