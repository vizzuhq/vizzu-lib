import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Values 1'] },
          y: { attach: ['Values 2'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          size: { attach: ['Values 3'] },
          label: { attach: ['Categ. Parent'] }
        },
        title: '1. Operations: Circle - Aggregated.',
        geometry: 'circle',
        legend: 'color'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ['Categ. Child'] },
        },
        title: '2. Operations: Circle - Drill Down.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Categ. Child'] },
          noop: { attach: ['Categ. Child'] }
        },
        title: '3. Operations: Circle - Drill Down.'
      },
      style: { plot: { marker: { label: { position: 'top', filter: 'lightness(0.1)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          noop: { detach: ['Categ. Child'] },
          size: { attach: ['Categ. Child'] }
        },
        title: '4. Operations: Circle - Aggregated.',
      },
      style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Categ. Child'] }
        },
        title: '5. Operations: Circle - Aggregated.'
      },
    }
  )
];

export default testSteps;