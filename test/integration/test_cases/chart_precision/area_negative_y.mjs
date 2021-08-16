import { data } from '/test/integration/test_data/chart_precision.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Parents'] },
          y: { attach: ['Childs', 'Values child'], range: '0,1.1,%' },
          label: { attach: ['Values child'] },
          lightness: { attach: ['Childs'] }
        },
        title: 'Chart Precision Area',
        geometry: 'area',
        legend: null
      },
      style: 
      { 
        plot: 
        { marker: 
          { label: 
            { position: 'top', filter: 'lightness(0)' }
          } 
         } 
      }
    }
  )
];

export default testSteps;