import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Year'] },
          y: { attach: ['Positive nums'], range: '0,1.1,%' },
          label: { attach: ['Positive nums'] },
        },
        title: 'Histogram with (-) Nums',
        align: 'none'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Threat'] },
          color: { attach: ['Threat'] }
        },
        title: 'Stacked',
        legend: 'color'
      },
      style:
      {
        plot:
        {
          paddingLeft: 120
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Threat', 'Positive nums'], attach: ['Negative nums'], range: '0,1.1,%' },
          x: { attach: ['Threat'] },
          label: { detach: ['Positive nums'], attach: ['Negative nums'] },
        },
        title: 'Grouped with (-) Nums',
        split: true,
      },
      style:
      { plot:
        { marker:
          { label:
            { 
              fontSize: 8, 
              position: 'top',
              orientation: 'vertical', 
              angle: -1*3.14 
            }
          }
        }
      }
    }
  )
];

export default testSteps;