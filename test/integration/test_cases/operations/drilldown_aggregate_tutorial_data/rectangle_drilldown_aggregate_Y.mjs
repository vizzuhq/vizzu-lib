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
        title: 'Operations: Rectangle - Aggregated.',
        legend: null
      },
      style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Categ. Parent'] },
          color: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Drill Down.',
        legend: "color", //Switching on the legend
      },
      style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Parent'] },
          color: { detach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Aggregated.',
        legend: null
      },
      style: { plot: { marker: { label: { position: 'above', filter: 'lightness(0.1)' } } } }
    }
  )
];

export default testSteps;