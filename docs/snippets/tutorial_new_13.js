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

 // 13. Animation settings
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
       title:'Animation settings'
     }
     }))
.then(chart => 
chart.animate({ 
  config: {
    title:'Default settings'
  },
})) 

// 4.4 - Group
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        y: { detach: ['Types'] },
        x: { attach: ['Types'] },
      },
    }
    }))

// 4.5 - Stack
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { detach: ['Types'] },
        y: { attach: ['Types'] },
      },
    }
    }))

.then(chart => 
  chart.animate({ 
    config: {
      title:'Custom animation settings for specific groups'
    },
  }))    

.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        y: { detach: ['Types'] },
        x: { attach: ['Types'] },
      },
    },
    },{
      y: {duration: 2, delay:2},
      style: {duration: 2, delay:4}
    }))

.then(chart => 
  chart.animate({ 
    config: {
      title:'Custom settings for the whole animation'
    },
  }))    
    

// 4.5 - Stack
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { detach: ['Types'] },
        y: { attach: ['Types'] },
      },
    }
    },{duration:1, easing:'linear',
    }))

.then(chart => 
  chart.animate({ 
    config: {
      title:'Custom settings for both'
    },
  }))    

// 4.5 - Stack
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { attach: ['Types'] },
        y: { detach: ['Types'] },
      },
    }
    },{duration:1, easing:'linear',
      y: {duration: 2, delay:2},
      style: {duration: 2, delay:4}
    }))
.


        catch((err) => {
          console.log(err);
        });

 

