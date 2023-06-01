import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
    let setDropshadow = event => {
      let ctx = event.renderingContext;
      ctx.shadowColor = "#00000060";
      ctx.shadowBlur = 9;
      ctx.shadowOffsetX = 3;
      ctx.shadowOffsetY = 3;
    }

    chart.on('plot-marker-draw', setDropshadow);
    chart.on('plot-marker-label-draw', setDropshadow);

    return chart.animate({
      data: data,
      config: {
        x: 'Joy factors',
        y: 'Value 2 (+)',
        color: 'Joy factors',
        label: 'Value 2 (+)',
        title: 'Drop-shadow on the markers'
      }
    })
  }
];

export default testSteps;