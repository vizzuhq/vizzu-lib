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
        title: 'Operations: Line - Filter.',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      data: {
        filter: record => record.Timeseries != '2019' && record.Timeseries != '2021'
      },
      config: {
        title: 'Operations: Line - Filtered.'
      },
    }
  )
];

export default testSteps;