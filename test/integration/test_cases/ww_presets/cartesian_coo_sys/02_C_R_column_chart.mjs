import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: {attach: 'Joy factors'},
          y: {attach: 'Value 2 (+)'},
          label: {attach: ['Value 2 (+)']},
        },
        title: 'Column Chart'
      },
      style: {
          plot: {
              paddingLeft: 70,
              yAxis: {
                  label: {
                     paddingRight: 10
                  }
              },
              xAxis: {
                  title: { paddingTop: 30 },
                  label: { paddingTop: 10 }
              }
          }
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;