import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
    chart.on('plot-marker-draw', event => {
      console.log(event)
      let ctx = event.renderingContext;
      let rect = event.data.rect;
      ctx.beginPath();
      ctx.moveTo(rect.pos.x, rect.pos.y);
      ctx.bezierCurveTo(
        rect.pos.x + rect.size.x / 2, rect.pos.y,
        rect.pos.x + rect.size.x / 2, rect.pos.y + rect.size.y,
        rect.pos.x + rect.size.x, rect.pos.y + rect.size.y);
      ctx.stroke();
      event.preventDefault();
    });

    return chart.animate({
      data: data,
      config: {
        x: { set: ['Year'] },
        y: { set: ['Value 5 (+/-)'] },
        title: 'Smooth Line Chart',
        geometry: 'line'
      }
    })
  }
];

export default testSteps;