import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  (chart) => {
    class Scroller {
      constructor(min, max) {
        this.min = min
        this.max = max
        this.window = max - min
        this.pos = null
      }

      drag(pos) {
        this.pos = pos
      }

      release() {
        this.pos = null
      }

      track(pos) {
        if (this.pos) {
          const delta = (this.pos - pos) * this.window
          this.min += delta
          this.max += delta
          this.pos = pos
        }
      }
    }

    const scroller = new Scroller(0.5, 5.5)

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

    chart.on('click', (event) => {
      event.preventDefault()
    })

    chart.on('pointerdown', (event) => {
      const convert = chart.getConverter('plot-area', 'canvas', 'relative')
      const rel = convert(event.detail.position)
      scroller.drag(rel.x)
      event.preventDefault()
    })

    chart.on('pointerup', (event) => {
      scroller.release()
      event.preventDefault()
    })

    chart.on('pointermove', (event) => {
      const convert = chart.getConverter('plot-area', 'canvas', 'relative')
      const rel = convert(event.detail.position)
      scroller.track(rel.x)
      throttle.call(() =>
        chart.animate(
          {
            x: {
              range: {
                min: scroller.min,
                max: scroller.max
              }
            }
          },
          { duration: '50ms', easing: 'linear' }
        )
      )
      event.preventDefault()
    })

    return chart.animate(
      {
        data,
        config: {
          x: {
            set: 'Year',
            range: {
              min: scroller.min,
              max: scroller.max
            }
          },
          y: 'Value 5 (+/-)',
          title: 'Mouse Scroll',
          geometry: 'line'
        }
      },
      0
    )
  },
  (chart) => {
    chart._cChart._call(chart._cChart._wasm._vizzu_pointerDown)(
      chart.render.ccanvas.getId(),
      0,
      250,
      150
    )
    chart._cChart._call(chart._cChart._wasm._vizzu_pointerMove)(
      chart.render.ccanvas.getId(),
      0,
      150,
      150
    )
    chart._cChart._call(chart._cChart._wasm._vizzu_pointerUp)(
      chart.render.ccanvas.getId(),
      0,
      150,
      150
    )
    return chart.anim
  }
]

export default testSteps
