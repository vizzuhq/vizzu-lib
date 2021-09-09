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

 // 1	Axes, title, tooltip - hiányzik az üres chart, mint kiindulási alap
  chart.initializing.then( 
 // 1.1 - set
 
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity'] },
        x: { set: ['Genres'] },
      },
    }
    }))

// 1.2 - set-tel áthelyezés
    .then(chart => 
      chart.animate({
        config: {
          channels: {
            x: { set: null },
            y: { set: ['Genres','Popularity'] },
             },
        },
      })
    )
//1.3 detach-attach conti
.then(chart => 
  chart.animate({
    config: {
      channels: {
        y: { detach: ['Popularity'] },
        x: { attach: ['Popularity'] },
         },
    },
  })
)

//1.4 title
.then(chart => 
  chart.animate({
      title:'My first chart'
  })
)


// 1.5 tooltip on
.then(chart => {
  chart.feature('tooltip',true);
  return chart;
}
)


.then(chart => 
  chart.animate({
      title:'Tooltip on'
  })
)

.


        catch((err) => {
          console.log(err);
        });

 

