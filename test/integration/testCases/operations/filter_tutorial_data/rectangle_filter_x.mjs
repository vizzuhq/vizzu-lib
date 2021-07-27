import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1', 'Categ. Parent'], },
          color: { attach: ['Categ. Parent'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Area - Filter.',
        legend: 'color'
      },
      style: { plot: { marker: { borderWidth: 1, borderOpacity: 1, label: { position: 'center', filter: 'lightness(0.8)' } } } }
    }
  ),
  chart => chart.animate(
    {
      data: {
        filter: record => record.Timeseries != '2019' && record.Timeseries != '2020'
      },
      descriptor: {
        title: 'Operations: Area - Filtered.'
      }
    }
  )
];

export default testSteps;