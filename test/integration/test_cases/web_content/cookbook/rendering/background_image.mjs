import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
  chart => 
  {
    var image = new Image();
    
    image.onload = function()
    {
      chart.on('background-draw', event => {
        event.renderingContext.drawImage(image, 0, 0, 
          event.data.rect.size.x, event.data.rect.size.y);
        event.preventDefault();
      });
    }

    image.src = 'data:image/gif;base64,R0lGODlhAwACAPIAAJLf6q/i7M/r8un0+PT6+/n8/QAAAAAAACH5BAQAAAAALAAAAAADAAIAAAMEWBMkkAA7';

    return chart.animate({
      data: data,
      config: {
        x: 'Joy factors',
        y: { set: 'Value 2 (+)', interlacing: false },
        title: 'Background image'
      }
    })
  }
];

export default testSteps;