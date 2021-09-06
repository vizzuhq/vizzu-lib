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

 // 2	Geometry
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity'] },
        x: { set: ['Genres'] },
      },
      title: 'Changing geometry',
    }
    }))

// 2.1 - area
    .then(chart => 
      chart.animate({
        config: {
          geometry: 'area',
          title: 'Geometry: area',
        },
      })
    )
// 2.2 - line
.then(chart => 
  chart.animate({
    config: {
      geometry: 'line',
      title: 'Geometry: line', 
      },
    })
)
// 2.3 - circle
.then(chart => 
  chart.animate({
    config: {
      geometry: 'circle',
      title: 'Geometry: circle',
         },
    })
)

// 2.4 - rectangle
.then(chart => 
  chart.animate({
    config: {
      geometry: 'rectangle',
      title: 'Geometry: rectangle - default',
         },
    })
)
.


        catch((err) => {
          console.log(err);
        });

 

