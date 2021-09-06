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

 // 7. Aggregate, drill-down
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { attach: ['Popularity','Types'] },
        x: { set: ['Genres'] },
        color:{attach:['Types']},
        label:{set: ['Popularity']},
      },
      title:'Aggregate, drill-down'
    }
    }))

// 7.1 - Stack
.then(chart =>
  chart.animate({
    config: {
      channels:{
        y:{attach: ['Genres']
        },
        x:{set: null},
      },
      title:'Stack'
    },
  })
)

// 7.2 - Aggregate
.then(chart =>
  chart.animate({
    config: {
      channels:{
        y:{detach: ['Genres']
        },
      },
      title: 'Aggregate elements'
    },
  })
)

// 7.3 - Drill-down
.then(chart =>
  chart.animate({
    config: {
      channels:{
        y:{attach: ['Genres']},
      },
      title:'Drill-down',
    },
  })
)

// 7.4 - Group
.then(chart =>
  chart.animate({
    config: {
      channels:{
        y:{detach: ['Genres']
        },
        x:{set: ['Genres']},
      },
      title:'Group'
    },
  })
)


.


        catch((err) => {
          console.log(err);
        });

 

