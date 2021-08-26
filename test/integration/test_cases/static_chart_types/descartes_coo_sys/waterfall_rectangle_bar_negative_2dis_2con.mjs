import { data } from '/test/integration/test_data/chart_types_eu.js';

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
      style: {
        plot: {
          yAxis: { label: { fontSize: 10, paddingRight: 20, } }, 
          paddingLeft: 100 
        }
      }
    }
  )
];

export default testSteps;