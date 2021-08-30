import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          y: {attach: ['Joy factors', 'Value 2 (+)'], range: '1.1,0,%'},
          color: {attach: ['Joy factors']},
          label: {attach: ['Value 2 (+)']},
        },
        title: 'Stacked Column Chart'
      }
    }
  )
];

export default testSteps;