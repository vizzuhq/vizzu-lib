import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          y: {attach: ['Threat', 'Positive nums'], range: '1.1,0,%'},
          color: {attach: ['Threat']},
          label: {attach: ['Positive nums']},
        },
        title: 'Column Chart Stacked'
      },
      style: 
      { 
        plot: 
        { 
          paddingLeft: 150
        }
      }
    }
  )
];

export default testSteps;