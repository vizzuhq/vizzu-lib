import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
  (chart) => {
    let labelToShow = ''

    chart.on('pointeron', (event) => {
      labelToShow = event.target.values['Value 5 (+/-)']
      event.preventDefault()
    })

    chart.on('plot-marker-label-draw', (event) => {
      if (event.target.value != labelToShow) event.preventDefault()
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
