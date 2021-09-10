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

// 13.1 - Group
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        y: { detach: ['Types'] },
        x: { attach: ['Types'] },
      },
    }
    }))

// 13.2 - Stack
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { detach: ['Types'] },
        y: { attach: ['Types'] },
      },
    }
    }))

//13.3 - Custom settings for selected groups
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

// 13.4 - Custom settings for the whole animation
.then(chart => 
  chart.animate({ 
    config: {
      title:'Custom settings for the whole animation'
    },
  }))    
    


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

// 13.4 - Custom settings for both

.then(chart => 
  chart.animate({ 
    config: {
      title:'Custom settings for both'
    },
  }))    


.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { attach: ['Types'] },
        y: { detach: ['Types'] },
      },
    }
    },
    {duration:1, easing:'linear',
      y: {duration: 2, delay:2},
      style: {duration: 2, delay:4}
    }))
.


        catch((err) => {
          console.log(err);
        });

 

