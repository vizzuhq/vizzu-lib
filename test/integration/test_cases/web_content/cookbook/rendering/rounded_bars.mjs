import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
    let roundRect = (ctx, rect, radius) => {
      let x = rect.pos.x;
      let y = rect.pos.y;
      let w = rect.size.x;
      let h = rect.size.y; 
      console.log(x,y,w,h)
      if (radius > w / 2) radius = w / 2;
      if (radius > h / 2) radius = h / 2;
      ctx.beginPath();
      ctx.moveTo(x + radius, y);
      ctx.lineTo(x + w - radius, y);
      ctx.quadraticCurveTo(x + w, y, x + w, y + radius);
      ctx.lineTo(x + w, y + h - radius);
      ctx.quadraticCurveTo(x + w, y + h, x + w - radius, y + h);
      ctx.lineTo(x + radius, y + h);
      ctx.quadraticCurveTo(x, y + h, x, y + h - radius);
      ctx.lineTo(x, y + radius);
      ctx.quadraticCurveTo(x, y, x + radius, y);
      ctx.closePath();
      ctx.fill();
    }

    chart.on('plot-marker-draw', event => {
      roundRect(event.renderingContext, event.data.rect, 15)
      event.preventDefault();
    });

    return chart.animate({
      data: data,
      config: {
        x: 'Joy factors',
        y: 'Value 2 (+)',
        color: 'Joy factors',
        label: 'Value 2 (+)',
        title: 'Rounded bars'
      }
    })
  }
];

export default testSteps;