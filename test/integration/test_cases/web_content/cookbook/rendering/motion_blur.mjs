import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  (chart) => {
    let images = []

    const drawImages = (ctx) => {
      ctx.drawImage(images[0], 0, 0)
      ctx.globalAlpha = 0.5
      images.map((image) => ctx.drawImage(image, 0, 0))
      return images.length > 0
    }

    const drawComplete = (event) => {
      const ctx = event.renderingContext
      const canvas = chart.getCanvasElement()
      const imageData = ctx.getImageData(0, 0, canvas.clientWidth, canvas.clientHeight)
      createImageBitmap(imageData).then((image) => {
        images.push(image)
      })
      if (images.length > 10) images.shift()
      drawImages(ctx)
    }

    chart.on('draw-complete', drawComplete)

    return chart.animate(
      {
        data,
        config: {
          x: 'Joy factors',
          y: 'Value 2 (+)',
          color: 'Joy factors',
          label: 'Value 2 (+)',
          title: 'Drop-shadow on the markers'
        }
      },
      '0.7s'
    )
  },

  (chart) =>
    chart
      .animate({
        coordSystem: 'polar',
        y: 'Joy factors',
        x: 'Value 2 (+)'
      })
      .then((chart) => {
        let handle = setInterval(() => {
          let ctx = chart.getCanvasElement().getContext('2d')
          if (!drawImages(ctx)) clearInterval(handle)
        }, 40)
      })
]

export default testSteps
