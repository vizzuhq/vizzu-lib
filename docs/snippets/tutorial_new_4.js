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

 // 4. Adding new categories to channels - group/stack
  chart.initializing.then( 
    // Induló állapot
       chart => chart.animate({ 
       data: data,
       config: {
         channels: {
           y: { set: ['Popularity'] },
           x: { set: ['Genres'] },
         },
      title: 'Adding categories - group/stack',
       }
       }))

  // 4.1 - nem jó megoldás
  .then(chart => 
    chart.animate({ 
      config: {
        channels: {
          x: { set: ['Genres','Types'] },
          color: { attach: ['Types'] },
        },
        title: 'The wrong way means no animation'
      }
      }))

  // 4.2 - vissza
  .then(chart => 
    chart.animate({ 
      config: {
        channels: {
          x: { detach: ['Types'] },
          color: { set: null },
        },
        title: 'Let\'s get back to where we were'
      }
      }))

// 4.3 - jó verzió step 1
  .then(chart => 
    chart.animate({ 
      config: {
        channels: {
          y: { attach: ['Types'] },
          color: { attach: ['Types'] },
        },
        title: 'The right way: add to the same channel where the conti is'
      }
      }))

// 4.4 - jó verzió step 2 = group
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        y: { detach: ['Types'] },
        x: { attach: ['Types'] },
      },
      title:'...then you can add it to another channel = group elements...'
    }
    }))

// 4.5 - stack
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { detach: ['Types'] },
        y: { attach: ['Types'] },
      },
      title: '...doing it the other way is how you stack your chart'
    }
    }))


.


        catch((err) => {
          console.log(err);
        });

 

