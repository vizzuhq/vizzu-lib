import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
    config: chart.constructor.presets.column({
      x:'Joy factors',
      y:'Value 2 (+)',
      title: 'Column Chart'
    }),
      style: {
          plot: {
              paddingLeft: '8em',
              yAxis: {
                  label: {
                     paddingRight: '0.8em'
                  }
              },
              xAxis: {
                  label: { paddingTop: '0.8em' }
              }
          }
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;