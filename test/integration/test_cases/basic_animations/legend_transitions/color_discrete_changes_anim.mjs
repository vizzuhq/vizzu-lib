import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          y: { attach: ['Negative nums'], range: '0,1.1,%' },
          x: { attach: ['Neg_huge'], range: '0,1.1,%' },
          color: { attach: ['Country'] },
          size: { attach: ['Neg_small'] },
          label: { attach: ['Negative nums'] }
        },
        title: 'Color Discrete series Changing Anim.',
        legend: 'color',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: {
            attach: ['Year']
          },
          label: { 
            detach: ['Negative nums'] 
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: {
            detach: ['Year'],
            attach: ['Country']
          },
          color: {
            detach: ['Country'],
            attach: ['Year'],
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: {
            detach: ['Country']
          },
          label: {
            attach:  ['Year']
          }
        }
      }
    }
  )
];

export default testSteps;