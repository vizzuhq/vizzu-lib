import { data } from '../../../../test_data/chart_types_eu.mjs'
import VideoCapture from '../../../../utils/vizzu-videocapture.mjs'

const testSteps = [
  (chart) => {
    chart.feature(new VideoCapture())

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
      chart.feature('video-capture').start()
    })

    anim.then((chart) => {
      chart
        .feature('video-capture')
        .stop()
        .then((dataURL) => {
          window.open(dataURL)
        })
    })

    return anim
  }
]

export default testSteps
