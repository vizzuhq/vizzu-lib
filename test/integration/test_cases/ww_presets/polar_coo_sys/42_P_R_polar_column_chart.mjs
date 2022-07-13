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
        title: 'Polar Column Chart',
        coordSystem: 'polar'
      },
      style: {
          plot: {
              paddingLeft: '3.8em',
              yAxis: {
                  label: {
                     paddingRight: '0em'
                  }
              },
              xAxis: {
                  title: { paddingTop: '2.4em' },
                  label: { paddingTop: '0.8em' }
              }
          }
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;