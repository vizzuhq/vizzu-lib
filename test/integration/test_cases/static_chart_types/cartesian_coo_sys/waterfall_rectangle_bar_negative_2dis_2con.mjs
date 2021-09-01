import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { attach: ['Year', 'Value 5 (+/-)'], range: '0,1.1,%' },
          y: { attach: ['Country', 'Year'] },
          color: { attach: ['Country'] },
          lightness: { attach: ['Value 6 (+/-)'] }
        },
        title: 'Sample Waterfall',
        orientation: 'vertical',
        align: 'none',
        legend: null
      },
      style: {
        plot: {
          yAxis: 
          { 
            label: 
            { 
              fontSize: 10,
              paddingRight: 20, } },
              paddingLeft: 100 
        }
      }
    }
  )
];

export default testSteps;