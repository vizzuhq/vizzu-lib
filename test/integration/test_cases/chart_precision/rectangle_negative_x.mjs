import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Childs', 'Values child'], range: '0,1.1,%' },
          y: { attach: ['Parents', 'Childs'] },
          label: { attach: ['Values child'] }
        },
        title: 'Chart Precision Rectangle (-) - X'
      }
    }
  )
];

export default testSteps;