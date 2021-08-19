import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Line - Orientation Change - No:1.',
        geometry: 'line',
        legend: "color",
 //       orientation: 'vertical'
      },
      style: {
        plot: {
          marker: {
            label: { position: 'top', filter: 'lightness(0.1)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Values 1'] },
          x: { attach: ['Values 1'] },
          color: { detach: ['Categ. Parent'] },
          color: { attach: ['Timeseries'] }
        },
        title: 'Operations: Line - Orientation Change - No:2',
        legend: "color",
//        orientation: 'horizontal'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          x: { detach: ['Values 1'] },
          y: { attach: ['Values 1'] },
          color: { detach: ['Timeseries'] },
          color: { attach: ['Categ. Parent'] }
        },
        title: 'Operations: Line - Orientation Change - No:3',
        legend: "color",
//        orientation: 'vertical'
      }
    }
  )
];

export default testSteps;