import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries', 'Categ. Parent'] },
          y: { attach: ['Values 1'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Line - Aggregated - X axis.',
        geometry: 'line',
        legend: null
      },
      style: { plot: { marker: { label: { position: 'top', filter: 'lightness(0.1)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ['Categ. Parent'] },
        },
        title: 'Operations: Line - Drill Down - X axis.',
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Categ. Parent'] },
          x: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Line - Drill Down - X axis.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ['Categ. Parent'] },
          x: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Line - Aggregated - X axis.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Line - Aggregated - X axis.'
      }
    }
  )
];

export default testSteps;