import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate({
    data: data,
    config: {
      channels: {
        x: { set: ['Timeseries'] },
        y: {
          set: ['Values 1', 'Categ. Parent'],
          range: {
            max: '400'  // Making the chart elements fill the whole of the y-axis as the default value is now 110%
          }
        },
        label: { set: ['Values 1'] },
        color: { set: ['Categ. Parent'] },
      },
      title: 'Stacked Area Chart',
      geometry: 'area'
    }
  }),
  chart => chart.animate({
    config: {
      channels: {
        y: { set: ['Values 1'] }
      },
      title: 'Line Chart',
      geometry: 'line'
    }
  }),
  chart => chart.animate({
    config: {
      channels: {
        y: {
          range: {
            max: 'auto' // Setting back the y-axis range to the default value.
          }
        }
      }
    }
  })
];

export default testSteps;