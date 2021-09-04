import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: {attach: ['Childs', 'Values parent'], range: { min: '0%', max: '110%' }},
          y: {attach: ['Parents']},
          label: { attach: ['Values parent']}
        },
        title: 'Chart Precision Rectangle - X'
      }
    }
  )
];

export default testSteps;