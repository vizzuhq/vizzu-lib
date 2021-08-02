import { data } from '/test/integration/test_data/chart_precision.js';

let styles = {
  plot: {
    marker: {
      label: { position: 'top', filter: 'lightness(0)' }
    }
  }
};

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Childs', 'Values child'], range: '0,1.1,%' },
          y: { attach: ['Parents'] },
          label: { attach: ['Values child'] },
          size: { attach: ['Values child'] }
        },
        title: 'Chart Precision Line (-) - X',
        geometry: 'line',
        legend: null
      },
      style: styles
    }
  )
];

export default testSteps;