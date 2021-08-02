import { data } from '/test/integration/test_data/chart_precision.js';

let styles = {
  data: { circleMinRadius: 5, circleMaxRadius: 0.05 },
  plot: {
    marker: {
      label: { fontSize: 10, position: 'top', filter: 'lightness(0.3)', numberFormat: 'grouped' }
    }
  }
};

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Parents'] },
          y: { attach: ['Childs', 'Values child'], range: '0,1.1,%' },
          label: { attach: ['Values child'] },
          size: { attach: ['Values child'] }
        },
        title: 'Chart Precision Circle',
        geometry: 'circle',
        legend: null
      },
      style: styles
    }
  )
];

export default testSteps;