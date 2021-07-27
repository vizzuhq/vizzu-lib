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
        title: 'Treemap - Aggregated.', //Setting the chart title
        legend: null, // Turning off the legend
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
        title: 'Treemap - Drill Down.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ["Categ. Child"] }
        },
        title: 'Treemap - Aggregated.'
      }
    }
  )
];

export default testSteps;