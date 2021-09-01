import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Parents', 'Childs'] },
          y: { attach: ['Parents', 'Childs', 'Values child'], range: '1.1,0,%' },
          color: { attach: ['Parents'] },
          lightness: { attach: ['Values child'] },
          label: { attach: ['Values child'] }
        },
        title: 'Waterfall (-) lightness',
        legend: 'lightness'
      }
    }
  )
];

export default testSteps;