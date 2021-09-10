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

 // 8. Orientation, split, polar
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { attach: ['Popularity','Types'] },
        x: { set: ['Genres'] },
        color:{set:['Types']},
        label:{attach: ['Popularity']},
      },
      title:'Switch orientation, split, polar coordinates'
    }
    }))
    

// 8.1 - change orientation
    .then(chart =>
      chart.animate({
        config: {
          channels:{
            y:{detach: ['Popularity']},
            x:{attach: ['Popularity']},
          },
          title: 'Switch the orienation = arrange by other axis'
        },
      })
    )

// 8.2 - split
.then(chart =>
  chart.animate({
    config: {
      split:true,
      title:'Split stacked values = show side-by-side'
    },
  })
)

// 8.3 - split off
.then(chart =>
  chart.animate({
    config: {
      split:false,
      title: 'Merge'
    },
  })
)

// 8.4 - aggregate
.then(chart =>
  chart.animate({
    config: {
      channels:{
        x:{detach: ['Genres']},
      },
      title:'Aggregate'
    },
  })
)

// 8.5 - polar
.then(chart =>
  chart.animate({
    config: {
      channels:{
        x:{range:{max:'133%'}},
        y:{range:{min:'-100%'}}
      },
      coordSystem: 'polar',
      title:'Polar coordinates',
    },
  })
)
.
        catch((err) => {
          console.log(err);
        });

 

