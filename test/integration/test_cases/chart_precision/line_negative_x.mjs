import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Values child'], range: '0,1.1,%' },
          y: { attach: ['Parents'] },
          label: { attach: ['Values child', 'Childs'] },
          size: { attach: ['Values child'] }
        },
        title: 'Chart Precision Line (-) - X',
        geometry: 'line'
      },
      style: 
      {
        plot: 
        {
          marker: 
          {
            label: 
            { 
              position: 'top'
            }
          }
        }
      }
    }
  )
];

export default testSteps;