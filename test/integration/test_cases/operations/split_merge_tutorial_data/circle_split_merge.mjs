import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Values 2'] },
          y: { attach: ['Values 1'], range: '0,1.1,%' },
          color: { attach: ['Categ. Parent'] },
          size: { attach: ['Values 3'] },
          label: { attach: ['Categ. Child','Values 1'] }
        },
        title: 'Operations: Circle - Merged.',
        legend: 'color',
        geometry: 'circle'
      },
      style:
      { plot:
        { marker:
          { label:
            { position: 'right', filter: 'lightness(0)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Categ. Parent', 'Categ. Child'] }
      },
        title: 'Operations: Circle - Splitted.',
        split: true,
      },
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Parent', 'Categ. Child'] }
      },
        title: 'Operations: Circle - Merged.',
        split: false,
      },
    }
  )
];

export default testSteps;