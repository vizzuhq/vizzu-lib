import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1', 'Categ. Parent'], },
          color: { attach: ['Categ. Parent'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Area - Filter.',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      data: {
        filter: record => record['Categ. Parent'] != 'A' && record['Categ. Parent'] != 'B'
      },
      config: {
        title: 'Operations: Area - Filtered.',
      }
    }
  )
];

export default testSteps;