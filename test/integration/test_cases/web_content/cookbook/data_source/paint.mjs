import { Mouse } from '../../../../utils/mouse.mjs'
import { lastAnim } from '../../../../utils/utils.mjs'

const testSteps = [
  (chart) => {
    const data = {
      series: [
        { name: 'index', type: 'dimension' },
        { name: 'x', type: 'measure' },
        { name: 'y', type: 'measure' }
      ]
    }

    let index = 0

    chart.on('click', (event) => {
      const convert = chart.getConverter('plot-area', 'canvas', 'relative')
      const rel = convert(event.detail.position)
      chart.animate(
        {
          data: {
            records: [[index++, rel.x, rel.y]]
          }
        },
        0.1
      )
    })

    return chart.animate({
      data,
      config: {
        x: { set: 'x', range: { min: 0, max: 1 } },
        y: { set: 'y', range: { min: 0, max: 1 } },
        geometry: 'circle',
        noop: 'index',
        title: 'Click the chart to put in data points!'
      }
    })
  },
  (chart) => {
    new Mouse(chart).down(100, 100).up(100, 100)
    return lastAnim(chart)
  },
  (chart) => {
    new Mouse(chart).down(300, 200).up(300, 200)
    return lastAnim(chart)
  }
]

export default testSteps
