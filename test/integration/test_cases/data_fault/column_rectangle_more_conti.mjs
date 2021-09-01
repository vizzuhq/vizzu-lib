import { data } from '/test/integration/test_data/data_missing_long_more_conti.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          y: { attach: ['Categ. Parent', 'Values 1'], range: '1.1,0,%' },
          x: { attach: ['Timeseries'] },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Column More Conti. elements'
      }
    }
  )
];

export default testSteps;