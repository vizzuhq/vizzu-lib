import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Values 1'] },
          y: { attach: ['Values 2'], range: '0,1.1,%' },
          color: { attach: ['Timeseries'] },
          size: { attach: ['Values 3'] },
          noop: { attach: ['Categ. Child'] }
        },
        title: 'Operations: Scatter Plot.',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {

      data: {
        filter: record => record.Timeseries != '2019' && record.Timeseries != '2021'
      },
      config: {
        title: 'Operations:  Scatter Plot - Filtered.'
      }
    }
  )
];

export default testSteps;