import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
config: {
        channels: {
          x: { set: ['Timeseries'] },
          y: { set: ['Values 1', 'Categ. Parent'], range: '0,400,1' },
          label: { set: ['Values 1'] },
          color: { set: ['Categ. Parent'] },

        },
        title: 'Total',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { set: ['Values 1'] }
        },
        title: 'Elements',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { range: '0,1.1,%'  }
        },
        title: 'Zoom into the Y axes'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { range: '0,400,1' }
        },
        title: 'Zoom back on the Y axes'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { set: ['Values 1', 'Categ. Parent'] }
        },
        title: 'Total',
        geometry: 'area'
      }
    }
  )
];

export default testSteps;