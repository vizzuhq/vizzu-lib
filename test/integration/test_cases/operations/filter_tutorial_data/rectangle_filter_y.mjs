import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Categ. Parent'] },
          y: { attach: ['Values 1', 'Timeseries'], },
          color: { attach: ['Timeseries'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Area - Filter.'
      }
    }
  ),
  chart => chart.animate(
    {
      data: {
        filter: record => record.Timeseries != '2018' && record.Timeseries != '2021'
      },
      config: {
        title: 'Operations: Area - Filtered.'
      }
    }
  )
];

export default testSteps;