import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
  plot: { axis: { label: { fontSize: 9, paddingLeft: 10, } }, paddingLeft: 105 },
};

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Year', 'Negative nums'], range: '0,1.1,%' },
          y: { attach: ['Country', 'Year'] },
          color: { attach: ['Country'] },
          lightness: { attach: ['Neg_huge'] }
        },
        title: 'Sample Waterfall',
        orientation: 'vertical',
        align: 'none',
        legend: null
      },
      style: styles
    }
  )
];

export default testSteps;