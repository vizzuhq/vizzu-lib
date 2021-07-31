import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Categ. Parent', 'Values 1'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Area - Merged.',
        legend: 'color',
        geometry: 'line'
      },
      style: {
        plot: {
          marker: {
            borderWidth: 0,
            borderOpacity: 1,
            label: { position: 'top', filter: 'lightness(0.1)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Operations: Area - Splitted.',
        split: true,
      },
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Operations: Area - Merged.',
        split: false,
      },
    }
  )
];

export default testSteps;