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



 // 11. Anim control
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
      title:'Animation control'
    }
    }))

    .then(chart => 
      chart.animate({ 
        config: {
          title:'Jumping from 25% to 75% progress during the animation'
        },
      }))    
    

// Setting a specific animation event
    .then(chart => 
    {
      chart.on('update', (event) => 
      {
          if(event.data.progress > 0.25)
          {
              chart.animation.pause();
              chart.off('update');
              setTimeout(() => {
                  chart.animation.seek('75%');
                  chart.animation.play();
              }, 1000);
          }
      });

      return chart.animate({
        config:{
          channels:{
            x: {
              attach: ['Types']
            },
            y: {
              detach: ['Types']
            }
          }
        }
      }
      );
    }
    )

.


        catch((err) => {
          console.log(err);
        });

 

