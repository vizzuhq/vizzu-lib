import { data } from '/test/integration/test_data/data_missing_long_more_conti.js';

let styles = {
  plot: {
    marker: {
      label: { position: 'center', filter: 'lightness(0.8)' }
    }
  }
};

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          y: { attach: ['Categ. Parent', 'Values 1'], range: '1.1,0,%' },
          x: { attach: ['Timeseries'] },
          color: { attach: ['Categ. Parent'] },
          label: { attach: ['Categ. Parent'] }
        },
        title: 'Column More Conti. elements',
        legend: 'color'
      },
      style: styles
    }
  )
];

export default testSteps;