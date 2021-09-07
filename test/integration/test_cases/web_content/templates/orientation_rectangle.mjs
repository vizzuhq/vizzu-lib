import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          x: { set: ['Timeseries'] },
          y: { set: ['Values 1', 'Categ. Parent'] },
          color: { set: ['Categ. Parent'] },
          label: { set: ['Values 1'] }
        },
        title: '...or back to rectangle.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { set: ['Categ. Parent'] },
          x: { set: ['Values 1', 'Timeseries'], range: {min:'-10%', max:'110%'} },
          
        },
        legend: null,
        title: 'Oriented',
      }
    },
    {x:{easing: 'ease-in' }}
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: { set: ['Values 1'] },
        },
        title: 'Values can be grouped by the other axis too.',
      }
    },
    {x:{easing: 'ease-out' }}
  )
];

export default testSteps;