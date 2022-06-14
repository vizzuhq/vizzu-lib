import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: {attach: 'Joy factors'},
          y: {attach: 'Value 2 (+)'}
        },
        title: 'Radial Column Chart',
        coordSystem: 'polar'
      },
      style: {
          plot: {
              paddingLeft: 50,
              yAxis: {
                  label: {
                     paddingRight: 0
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