import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          size: { attach: ['Values 1', 'Timeseries', 'Categ. Parent'] },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Circle - Groupped.',
        geometry: 'rectangle',
        legend: "color"
      }

    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Categ. Parent'] },
          shape: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Circle - Stacked.'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ['Categ. Parent'] },
          shape: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Circle - Groupped.'
      }
    }
  )
];

export default testSteps;