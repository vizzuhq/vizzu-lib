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

 // 9. Filtering
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity','Types'] },
        x: { set: ['Genres'] },
        color:{attach:['Types']},
        label:{attach:['Popularity']}
      },
      title:'Filtering data'
    }
    }))
    

// 9.1 - filter be
    .then(chart =>  chart.animate({
        data: {
            filter: record => record["Genres"] == 'Pop' || record["Genres"] == 'Metal', 
        },
        config:{
          title: 'Filter by one category'
        }
      })
    )

 //9.2 - másik filter be
    .then(chart =>  chart.animate({
      data: {
        filter: record => (record["Genres"] == 'Pop' || record["Genres"] == 'Metal') && record["Types"] == 'Smooth' 
      },
      config:{
        title:'Filter by two categories'
      }
    })
    )

// 9.3 - filter ki
    .then(chart =>  chart.animate({
      data: {
        filter: null, 
      },
      config:{
        title:'Filter off'
      }
    })
    )
.


        catch((err) => {
          console.log(err);
        });

 

