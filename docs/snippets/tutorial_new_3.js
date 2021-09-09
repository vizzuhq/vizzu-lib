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

 // 3. Channels & legend
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity'] },
        x: { set: ['Genres'] },
      },
      title: 'Channels & legend',
    }
    }))

// 3.1 - label
    .then(chart => 
      chart.animate({
        config: {
          channels:{
            label: {
              attach: ['Popularity'],
            }
          },
          title: 'Label'
             },
        })
    )

// 3.2 - lightness
.then(chart => 
  chart.animate({
    config: {
      channels:{
        lightness: {
          attach: ['Popularity'],
        }
      },
      legend:'lightness',
      title: 'Lightness - legend on' 
         },
    })
)

// 3.3 - color
.then(chart => 
  chart.animate({
    config: {
      channels:{
        lightness: {
          set: null,
        },
        color:{
          attach:['Genres']
        }
      },
      legend: 'color',
      title: 'Color' 
         },
    })
)

// 3.4 - size
.then(chart => 
  chart.animate({
    config: {
      channels:{
        size:{
          set:['Popularity']
        }
      }, 
      geometry:'circle',
      title: 'Size - change of geometry required'
         },
    })
)

.


        catch((err) => {
          console.log(err);
        });

 

