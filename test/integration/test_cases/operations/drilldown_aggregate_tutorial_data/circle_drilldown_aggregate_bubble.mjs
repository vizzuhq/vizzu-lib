import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          label: { attach: ['Values 1'] },
          color: { attach: ['Categ. Parent'] },
          size: { attach: ['Values 1'] }
        },
        title: 'Bubble - Aggregated.',
        geometry: 'circle',
        legend: null
      },
      style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ['Categ. Child'] }
        },
        title: 'Bubble - Drill Down.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ["Categ. Child"] }
        },
        title: 'Bubble - Aggregated.',
      }
    }
  )
];

export default testSteps;