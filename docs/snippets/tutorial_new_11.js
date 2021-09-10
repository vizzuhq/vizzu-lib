import {data} from './tutorial_data.js';
import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';


  let chart = new Vizzu('myVizzu');

  let slider = document.getElementById("myRange");
  
  slider.oninput = (e)=>
  {
    let t = e.target.value;
    chart.animation.pause();
    chart.animation.seek(t/10 + '%');
    
  };

 // 11. Palette, font size
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity','Types'] },
        x: { attach: ['Genres'] },
        color:{attach:['Types']},
        label:{set: ['Popularity']},
      },
      title:'Style settings'
    }
    }))
    

// 11.1 - palette
    .then(chart =>  chart.animate({
      config:{
        title: 'Color palette'
      },
      style: { 
        plot: { 
          marker: {
          colorPalette: '#9355e8FF #123456FF #BDAF10FF'
          },
        }
      }
      })
    )


// 11.2 - title font size
.then(chart =>  chart.animate({
  config: {
    title: 'Title font size'
  },
  style: { title: {
    fontSize: 50
  }
  }
  })
)

// 11.3 - title font size back to default
.then(chart =>  chart.animate({
  config: {
    title: 'Title font size - back to default'
  },
  style: { title: {
    fontSize: null
  }
  }
  })
)


// 11.4 - all font sizes
.then(chart =>  chart.animate({
  config: {
    title: 'Setting all font sizes simultaneously'
  },
  style: { fontSize: 20
  }
  })
)

.


        catch((err) => {
          console.log(err);
        });

 

