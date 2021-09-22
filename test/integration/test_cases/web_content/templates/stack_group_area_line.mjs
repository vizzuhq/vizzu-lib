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
            max: '400'  // Keep the yAxis-range during the next animation phase.
          }
        },
        label: { set: ['Values 1'] },
        color: { set: ['Categ. Parent'] },
      },
      title: 'Total',
      geometry: 'area'
    }
  }),
  chart => chart.animate({
    config: {
      channels: {
        y: { set: ['Values 1'] }
      },
      title: 'The elements separatelly',
      geometry: 'line'
    }
  }),
  chart => chart.animate({
    config: {
      channels: {
        y: {
          range: {
            max: '110%' // Set back the default yAxis-range.
          }
        }
      }
    }
  })
];

export default testSteps;