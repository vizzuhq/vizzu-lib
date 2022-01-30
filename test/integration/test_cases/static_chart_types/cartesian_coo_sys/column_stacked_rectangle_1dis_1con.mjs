import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          y: {attach: ['Joy factors', 'Value 2 (+)'], range: { min: '110%', max: '0%' }},
          color: {attach: ['Joy factors']},
          label: {attach: ['Value 2 (+)']},
        },
        title: 'Column Chart Stacked'
      }
    }
  )
];

export default testSteps;