import { data } from '/test/integration/test_data/cube_test_data.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          y: { set: ['Genres', 'Popularity'], range: { min: '110%', max: '0%' } },
          x: { set: ['Types'] },
          color: { set: ['Genres'] },
          label: { set: ['Popularity'] }
        },
        title: 'Data Cube'
      }
    }
  )
];

export default testSteps;