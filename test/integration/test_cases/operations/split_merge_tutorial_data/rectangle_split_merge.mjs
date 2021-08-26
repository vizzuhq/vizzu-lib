import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Categ. Parent', 'Values 1'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Values 1'] }
        },
        title: 'Operations: Rectangle - Merged.',
        geometry: 'rectangle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Operations: Rectangle - Splitted.',
        split: true,
      },
      style:
      { plot:
        { marker:
          { label:
            { position: 'top'
            }
          }
        } 
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Operations: Rectangle - Merged.',
        split: false
      },
      style:
      { plot:
        { marker:
          { label:
            { position: null 
            }
          }
        } 
      }
    }
  )
];

export default testSteps;