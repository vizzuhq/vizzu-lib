import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
          x: { attach: ['Value 6 (+/-)'], range: '0,1.1,%' },
          color: { attach: ['Country'] },
          size: { attach: ['Value 4 (+/-)'] },
          label: { attach: ['Value 5 (+/-)'] }
        },
        title: 'Color Discrete series Changing Anim.',
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
            detach: ['Value 5 (+/-)'] 
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