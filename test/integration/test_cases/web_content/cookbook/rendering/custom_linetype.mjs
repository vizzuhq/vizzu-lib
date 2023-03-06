import { data_4 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  async chart => 
  {
    let tinycolor = (await import('https://esm.sh/tinycolor2')).default;

    function setDash(ctx)
    {
      const dashes = [ [3, 17], [6, 14], [9, 11], [12, 8], [15, 5]];

      let color = ctx.fillStyle; 
      if (!tinycolor(color).isValid()) return;

      let series = tinycolor(color).getBrightness();

      ctx.setLineDash(dashes[series]);

      ctx.strokeStyle = '#666666';
    }

    chart.on('legend-marker-draw', event => 
    {
      let ctx = event.renderingContext;

      setDash(ctx);

      let rect = event.data.rect;

      ctx.beginPath();
      ctx.moveTo(rect.pos.x, rect.pos.y + rect.size.y/2);
      ctx.lineTo(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y/2);
      ctx.stroke();

      event.preventDefault();
    });

    chart.on('plot-marker-draw', event => 
    {
      let ctx = event.renderingContext;
      
      setDash(ctx);

      let rect = event.data.rect;

      ctx.beginPath();
      ctx.moveTo(rect.pos.x, rect.pos.y);
      ctx.lineTo(
        rect.pos.x + rect.size.x, rect.pos.y + rect.size.y);
      ctx.stroke();

      event.preventDefault();
    });

    return chart.animate({
      data: data_4,
      config: {
        x: 'Year',
        y: 'Value 5 (+/-)',
        color: 'Country',
        geometry: 'line',
        title: 'Custom line types'
      },
      style: { "plot.marker.colorPalette": '#000000 #010101 #020202 #030303 #040404'}
    })
  }
];

export default testSteps;