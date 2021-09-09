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

 // 6. Align & range
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity','Types']},
        x: { set: ['Genres'] },
        color:{set:['Types']},
        label:{attach: ['Popularity']},
      },
      title: 'Alignment & axis ranges'
    }
    }))

// 6.1 - align center
.then(chart =>
  chart.animate({
    config: {
      align:'center',
      title:'Align: center'
    },
  })
)
// 6.2 - align stretch
.then(chart =>
  chart.animate({
    config: {
      align:'stretch',
      title:'Align: stretch = % view'
    },
  })
)

// 6.3 - align none
.then(chart =>
  chart.animate({
    config: {
      align:'none',
      title: 'Align: none - default'
    },
  })
)

// 6.4 - range y %
.then(chart =>
  chart.animate({
    config: {
      channels:{
        y:{range:{max:'150%'}}
      },
      title:'Axis range set proportionally to shown values'
    },
  })
)

// 6.5 - range x 1
.then(chart =>
  chart.animate({
    config: {
      channels:{
        x:{range:{min:-2,max:3}}
      },
      title:'Axis range set explicitly on an axis with discrete series'
    },
  })
)

.


        catch((err) => {
          console.log(err);
        });

 

