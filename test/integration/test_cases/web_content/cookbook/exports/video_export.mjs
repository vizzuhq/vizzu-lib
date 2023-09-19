import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  async (chart) => {
    const recordedChunks = []
    const stream = chart.getCanvasElement().captureStream(30 /* fps */)
    const mediaRecorder = new MediaRecorder(stream)

    mediaRecorder.ondataavailable = (e) => {
      recordedChunks.push(e.data)
    }

    mediaRecorder.onstop = (event) => {
      const blob = new Blob(recordedChunks, {
        type: 'video/webm'
      })
      window.open(URL.createObjectURL(blob))
    }

    const anim = chart.animate({
      data,
      config: {
        x: 'Year',
        y: ['Value 2 (+)', 'Joy factors'],
        color: 'Joy factors',
        title: 'Video Export'
      }
    })

    anim.activated.then(() => {
      mediaRecorder.start()
    })

    anim.then((chart) => {
      mediaRecorder.stop()
    })

    return anim
  }
]

export default testSteps
