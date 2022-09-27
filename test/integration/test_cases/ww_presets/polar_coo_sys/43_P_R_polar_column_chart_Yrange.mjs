import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: {attach: 'Joy factors'},
          y: {attach: 'Value 2 (+)', range: {min: '-50%'}}
        },
        title: 'Polar Column Chart (Yrange)',
        coordSystem: 'polar'
      }
    }
  ),
  chart => chart.feature('tooltip',true)
];

export default testSteps;