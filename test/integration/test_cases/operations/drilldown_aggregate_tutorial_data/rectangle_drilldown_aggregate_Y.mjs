import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Aggregated.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { attach: ['Categ. Parent'] },
          color: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Drill Down.',
        legend: "color"
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { detach: ['Categ. Parent'] },
          color: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Aggregated.',
        legend: null
      }
    }
  )
];

export default testSteps;