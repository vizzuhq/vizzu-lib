import { data } from '../../../../test_data/tutorial.mjs';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: 'Timeseries',
          y: ['Values 1', 'Categ. Parent'],
          color: 'Categ. Parent',
          label: 'Values 1'
        },
        title: 'Operations: Area - Orientation Change - No:1.',
        geometry: 'area',
        orientation: 'vertical'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: ['Timeseries', 'Values 1'],
          y: 'Categ. Parent',
          color: 'Timeseries'
        },
        title: 'Operations: Area - Orientation Change - No:2',
        orientation: 'horizontal'
      }
    }
  )
];

export default testSteps;