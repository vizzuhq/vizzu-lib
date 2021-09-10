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

 // 12. Layout
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity','Types'] },
        x: { attach: ['Genres'] },
        color:{set:['Types']},
        label:{attach: ['Popularity']},
      },
      title:'Layout'
    }
    }))
    

// 12.1 - plot, title, legend background
    .then(chart =>  chart.animate({
      config:{
        title:'Plot, title and legend background'
      },
      style: { 
        title: {
        backgroundColor: '#A0A0A0'
        },
        plot:{
        backgroundColor: '#C0C0C0'
        },
        legend:{
        backgroundColor: '#808080'
        },
      }
      })
    )

// 12.2 - legend Width
    .then(chart =>  chart.animate({
      config:{
        title:'Legend width'
      },
      style: { 
        legend:{
        width: 50
        },
      }
      })
    )

// 12.3 - legend Width vissza
.then(chart =>  chart.animate({
  style: { 
    legend:{
    width: null
    },
  }
  })
)

// 12.4 - title Padding 
.then(chart =>  chart.animate({
  config:{
    title:'Title padding'
  },
  })
)


.then(chart =>  chart.animate({
  style: { 
    title:{
    paddingTop: 20,
    paddingBottom: 20,
    paddingLeft: 200,
    },
  }
  })
)

// 12.5 - title Padding vissza
.then(chart =>  chart.animate({
  style: { 
    title:{
    paddingTop: null,
    paddingBottom: null,
    paddingLeft: null,
    },
  }
  })
)

// 12.6 - plot Padding
.then(chart =>  chart.animate({
  config:{
    title:'Plot padding'
  },
  style: { 
    plot:{
      paddingLeft:100,
      paddingRight: 100,
    },
  }
  })
)

// 12.7 - plot Padding vissza
.then(chart =>  chart.animate({
  style: { 
    plot:{
      paddingLeft: null,
      paddingRight: null,
    },
  }
  })
)

.


        catch((err) => {
          console.log(err);
        });

 

