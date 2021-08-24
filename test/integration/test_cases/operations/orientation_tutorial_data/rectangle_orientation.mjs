import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1', 'Categ. Parent'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Orientation Change - No:1.',
        legend: "color"
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Values 1'] },
          x: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Orientation Change - No:2',
        legend: "color"
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          x: { detach: ['Values 1'] },
          y: { attach: ['Values 1'] },
          color: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Rectangle - Orientation Change - No:3',
        legend: "color"
      }
    }
  )
];

export default testSteps;