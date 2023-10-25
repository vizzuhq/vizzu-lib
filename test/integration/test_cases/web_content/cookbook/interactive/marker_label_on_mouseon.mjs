import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  (chart) => {
    const labelToShow = { value: '' }

    chart.on('pointeron', (event) => {
      if (event.target?.tagName === 'plot-marker') {
        labelToShow.value = event.target.values['Value 5 (+/-)']
        chart.forceUpdate()
      } else {
        labelToShow.value = ''
        chart.forceUpdate()
      }
    })

    chart.on('plot-marker-label-draw', (event) => {
      if (event.target.parent.values['Value 5 (+/-)'] !== labelToShow.value) event.preventDefault()
    })

    return chart.animate({
      data,
      config: {
        x: 'Year',
        y: 'Value 5 (+/-)',
        label: 'Value 5 (+/-)',
        title: 'Label shown on mouse-on'
      }
    })
  }
]

export default testSteps
