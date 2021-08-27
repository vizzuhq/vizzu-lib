import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Parents', 'Childs'] },
          y: { attach: ['Parents', 'Childs', 'Values child'], range: '1.1,0,%' },
          color: { attach: ['Values child'] }
        },
        title: 'Waterfall (-) Color Gradient',
        legend: 'color'
      }
    }
  )
];

export default testSteps;