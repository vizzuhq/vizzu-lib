import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: {attach: ['Childs', 'Values parent'], range: '0,1.1,%'},
          y: {attach: ['Parents']},
          label: { attach: ['Values parent']}
        },
        title: 'Chart Precision Rectangle - X'
      }
    }
  )
];

export default testSteps;