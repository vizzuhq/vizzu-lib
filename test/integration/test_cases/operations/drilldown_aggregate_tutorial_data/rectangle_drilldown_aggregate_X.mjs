import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Aggregated - X axis.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Categ. Parent'] },
          color: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Drill Down - X axis.',
        legend: "color"
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Parent'] },
          x: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Drill Down - X axis.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Categ. Parent'] },
          x: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Aggregated - X axis.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Parent'] },
          color: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Aggregated - X axis.',
        legend: null
      }
    }
  )
];

export default testSteps;