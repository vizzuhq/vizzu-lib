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
            min: '0',
            max: '400'
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
      title: 'Elements',
      geometry: 'line'
    }
  }),
  chart => chart.animate({
    config: {
      channels: {
        y: {
          range: {
            min: '0%',
            max: '110%'
          }
        }
      },
      title: 'Zoom into the Y axes'
    }
  })
];

export default testSteps;