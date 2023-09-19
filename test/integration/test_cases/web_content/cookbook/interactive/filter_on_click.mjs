import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  (chart) => {
    const clickHandler = (event) => {
      const filter =
        event.target && event.target.tagName === 'plot-marker'
          ? (record) => record.Year === event.target.categories.Year
          : null

      chart.animate({ data: { filter } })

      event.preventDefault()
    }

    chart.on('click', clickHandler)

    return chart.animate({
      data,
      config: {
        x: 'Year',
        y: 'Value 2 (+)',
        label: 'Value 2 (+)',
        title: 'Filter on click'
      }
    })
  },
  (chart) => {
    chart.module._vizzu_pointerDown(0, 150, 150)
    chart.module._vizzu_pointerUp(0, 150, 150)
    return chart.anim
  },
  (chart) => {
    chart.module._vizzu_pointerDown(0, 100, 100)
    chart.module._vizzu_pointerUp(0, 100, 100)
    return chart.anim
  }
]

export default testSteps
