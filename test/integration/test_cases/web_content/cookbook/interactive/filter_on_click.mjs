import { data } from '../../../../test_data/chart_types_eu.mjs'
import { Mouse } from '../../../../utils/mouse.mjs'
import { lastAnim } from '../../../../utils/utils.mjs'

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
    new Mouse(chart).down(150, 150).up(150, 150)
    return lastAnim(chart)
  },
  (chart) => {
    new Mouse(chart).down(100, 100).up(100, 100)
    return lastAnim(chart)
  }
]

export default testSteps
