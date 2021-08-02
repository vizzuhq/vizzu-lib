import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          label: { attach: ['Values 1'] },
          color: { attach: ['Timeseries'] },
          size: { attach: ['Values 1', 'Categ. Child'] }
        },
        title: 'Operation: Bubble Chart.',
        geometry: 'circle',
        legend: null
      },
      style: { plot: { marker: { label: { position: 'center', filter: 'lightness(0.8)' } } } }, //Set the labels to show on top of the bars.
    }
  ),
  chart => chart.animate(
    {
      data: {
        filter: record => record.Timeseries != '2019' && record.Timeseries != '2020'
      },
      descriptor: {
        title: 'Operation: Bubble - Filtered.',

      }
    }
  )
];

export default testSteps;
