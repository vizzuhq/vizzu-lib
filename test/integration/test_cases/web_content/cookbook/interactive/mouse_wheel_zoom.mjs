import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  (chart) => {
    class Zoomer {
      constructor(min, max) {
        this.min = min
        this.max = max
        this.finalMin = min
        this.finalMax = max
        this.pos = null
      }

      trackPos(pos) {
        this.pos = pos
      }

      zoom(factor) {
        const ref = this.min + this.pos * (this.max - this.min)
        this.min = ref - (1 + factor) * (ref - this.min)
        this.max = ref + (1 + factor) * (this.max - ref)
        if (this.min < this.finalMin) this.min = this.finalMin
        if (this.max > this.finalMax) this.max = this.finalMax
      }
    }

    const zoomer = new Zoomer(0.5, 20.5)

    class Throttle {
      constructor() {
        this.finished = true
        this.next = null
      }

      call(func) {
        if (!this.finished) {
          this.next = func
        } else {
          this.finished = false
          func().then(() => {
            this.finished = true
            if (this.next !== null) {
              const f = this.next
              this.next = null
              this.call(f)
            }
          })
        }
      }
    }

    const throttle = new Throttle()

    chart.getCanvasElement().addEventListener('wheel', (event) => {
      event.preventDefault()
    })

    chart.on('wheel', (event) => {
      zoomer.zoom(-event.detail.delta / 200)
      throttle.call(() =>
        chart.animate(
          {
            x: {
              range: {
                min: zoomer.min,
                max: zoomer.max
              }
            }
          },
          { duration: '50ms', easing: 'linear' }
        )
      )
    })

    chart.on('pointermove', (event) => {
      const convert = chart.getConverter('plot-area', 'canvas', 'relative')
      const rel = convert(event.detail.position)
      zoomer.trackPos(rel.x)
    })

    return chart.animate(
      {
        data,
        config: {
          x: {
            set: 'Year',
            range: {
              min: zoomer.min,
              max: zoomer.max
            }
          },
          y: 'Value 5 (+/-)',
          title: 'Mouse Wheel Zoom',
          geometry: 'line'
        }
      },
      0
    )
  },
  (chart) => {
    chart._callOnChart(chart.module._vizzu_pointerMove)(chart.render.ccanvas.id, 0, 250, 150)
    chart._callOnChart(chart.module._vizzu_wheel)(chart.render.ccanvas.id, 50)
    return chart.anim
  }
]

export default testSteps
