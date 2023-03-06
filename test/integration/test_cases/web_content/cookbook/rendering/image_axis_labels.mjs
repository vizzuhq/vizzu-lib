import { data_4 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  async chart => 
  {
    let tinycolor = (await import('https://esm.sh/tinycolor2')).default;

    function urlToImage(url)
    {
      return new Promise(resolve => {
        const image = new Image();
        image.addEventListener('load', () => { resolve(image); });
        image.src = url; 
      });
    }

    const flags = {
      Malta: await urlToImage("data:image/gif;base64,R0lGODlhGQARAPMAAM8UK+KzudDQ0NfX1/rp6/vs7vzv8fzx8////wAAAAAAAAAAAAAAAAAAAAAAAAAAACH/C05FVFNDQVBFMi4wAwEAAAAh/wtJbWFnZU1hZ2ljaw1nYW1tYT0wLjQ1NDU1ACH+IENyZWF0ZWQgd2l0aCBlemdpZi5jb20gR0lGIG1ha2VyACH5BAQUAP8ALAAAAAAZABEAAAQ6EMkzKwog652tv1wIFIQwkB4mbkZhtunKfpUqj/Rky7l+dz3QLyi8EXer4w/XQ4qUw6AzBDVKl1VZBAA7"),
      Lithuania: await urlToImage("data:image/gif;base64,R0lGODlhGQARAPIAAAJqRIA/NcEnLcQmLaedJf25EwAAAAAAACH5BAQUAP8ALAAAAAAZABEAAAMyWLrc/jDKSau9jejNu+dAKI5kaZ5oqq5s65JBLM90PQ94ru98L/zAoHBILBqPyKRymAAAOw=="),
      Germany: await urlToImage("data:image/gif;base64,R0lGODlhGQAPAPIAAAAAAB4AAMUAAN8AAOEbAPu4AP/PAAAAACH5BAQUAP8ALAAAAAAZAA8AAAMzCLrc/jDKSasMOOvNs/hgKI7jYJ5oqq5s674tIc90bctFru987xvAoHBILBqPyKRySUwAADs="),
      Bulgaria: await urlToImage("data:image/gif;base64,R0lGODlhGQAPAPIAAL80HdYmEtglER2JYwCVbSGif+Ly7f///yH5BAQUAP8ALAAAAAAZAA8AAAM3eLrc/jDKSas0OOvNc/lgKI4iYZ5oqq5s677tIM90bc9Aru98zwvAoHBILAaOyKRyyWw6n1BlAgA7")
    };

    chart.on('plot-axis-label-draw', event => 
    {
      if (!isNaN(event.data.text)) return; // do nothing for x axis
      let country = event.data.text;
      let rect = event.data.rect;
      let ctx = event.renderingContext;
      ctx.globalAlpha = tinycolor(ctx.fillStyle).getAlpha(); // support fade-in
      ctx.drawImage(flags[country], 
        rect.pos.x + rect.size.x + 7, rect.pos.y, 
        rect.size.y * 1.2, rect.size.y);
      ctx.globalAlpha = 1;
    });

    return chart.animate({
      data: data_4,
      config: {
        x: 'Value 2 (+)',
        y: 'Country',
        title: 'Images in the axis labels'
      },
      style: { "plot.yAxis.label.paddingRight": 30 } // making space for the flags
    })
  }
];

export default testSteps;
