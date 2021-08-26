import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Parents'] },
          y: { attach: ['Values child'], range: '0,1.1,%' },
          label: { attach: ['Childs', 'Values child'] },
          size: { attach: ['Values child'] }
        },
        title: 'Chart Precision Circle',
        geometry: 'circle'
      }
    }
  )
];

export default testSteps;