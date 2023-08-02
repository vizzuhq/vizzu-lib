import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  async chart => 
  {
    await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js');

    chart.on('plot-marker-draw', event => 
    {
      let ctx = event.renderingContext;
      
      let color = ctx.fillStyle; 
      if (!tinycolor(color).isValid()) return;

      let rect = event.data.rect;
      
      let grd = ctx.createLinearGradient(
        0, rect.pos.y, 
        0, rect.pos.y + rect.size.y);
      
      grd.addColorStop(0, tinycolor(color).darken(30));
      grd.addColorStop(0.5, tinycolor(color).darken(20));
      grd.addColorStop(1, tinycolor(color).darken(0));

      ctx.fillStyle = grd;
      ctx.strokeStyle = grd;
    });

    return chart.animate({
      data: data,
      config: {
        x: 'Joy factors',
        y: 'Value 2 (+)',
        color: 'Joy factors',
        title: 'Gradient on the markers'
      }
    })
  }
];

export default testSteps;