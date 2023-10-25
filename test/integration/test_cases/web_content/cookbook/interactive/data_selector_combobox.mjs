import { data } from '../../../../test_data/chart_types_eu.mjs'
import { lastAnim } from '../../../../utils/utils.mjs'

const testSteps = [
  (chart) => {
    const select = document.createElement('select')
    select.style = 'position: absolute; top: 40px; left: 300px;'
    chart.getCanvasElement().parentElement.appendChild(select)

    const options = ['Value 2 (+)', 'Value 3 (+)', 'Value 1 (+)']
    for (let i = 0; i < options.length; i++) {
      const option = document.createElement('option')
      option.value = options[i]
      option.text = options[i]
      select.appendChild(option)
    }

    select.addEventListener('change', (event) => {
      chart.animate({
        data,
        config: {
          y: event.target.value,
          label: event.target.value
        }
      })
    })

    return chart.animate(
      {
        data,
        config: {
          x: 'Joy factors',
          y: 'Value 2 (+)',
          color: 'Joy factors',
          label: 'Value 2 (+)'
        }
      },
      0
    )
  },
  (chart) => {
    document.querySelector('select').value = 'Value 3 (+)'
    const event = new Event('change')
    document.querySelector('select').dispatchEvent(event)
    return lastAnim(chart)
  }
]

export default testSteps
