import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1', 'Categ. Parent'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Orientation Change - No:1.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { detach: ['Values 1'] },
          x: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Orientation Change - No:2'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: { detach: ['Values 1'] },
          y: { attach: ['Values 1'] },
          color: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Orientation Change - No:3'
      }
    }
  )
];

export default testSteps;