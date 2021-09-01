import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          label: { attach: ['Values 1'] },
          color: { attach: ['Categ. Parent'] },
          size: { attach: ['Values 1'] }
        },
        title: 'Bubble - Aggregated.',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { attach: ['Categ. Child'] }
        },
        title: 'Bubble - Drill Down.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { detach: ["Categ. Child"] }
        },
        title: 'Bubble - Aggregated.',
      }
    }
  )
];

export default testSteps;