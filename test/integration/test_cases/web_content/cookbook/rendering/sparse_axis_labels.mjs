import { data } from '../../../../test_data/music_industry_history_1.mjs';

const testSteps = [
  chart => 
  {
    chart.on('plot-axis-label-draw', event => {
      let year = parseFloat(event.data.text);
      if (!isNaN(year) && year % 5 != 0)
        event.preventDefault();
    });

    return chart.animate(
    {
      data: data,
      config: {
          y: 'Revenue [m$]',
          x: { set: 'Year', ticks: true }, 
          title: 'Every 5th label shown on X'
      }
    })
  }
];

export default testSteps;
