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

 // 5. Sorting
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { set: ['Popularity','Types'] },
        x: { set: ['Genres'] },
        color:{attach:['Types']},
        label:{set: ['Popularity']},
      },
      title: 'Sort options - default: as in the data'
    }
    }))


// 5.1 - sort by value
.then(chart =>
  chart.animate({
    config: {
      sort:'byValue',
      title: 'Switch to ascending order...'
    },
  })
)

// 5.2 - reverse
.then(chart =>
  chart.animate({
    config: {
      reverse: true,
      title: '...or descending order.'
    },
  })
)

// 5.3 - original
.then(chart =>
  chart.animate({
    config: {
      sort:'none',
      reverse: false,
      title: 'Let\'s get back to where we were'
    },
  })
)

// 5.4 - két diszkrét esetén? a sorrend számít
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        y: { detach: ['Types'] },
        x: { set: ['Genres','Types'] },
      },
      title: 'With two discretes on one axis...'
    }
    }))


// 5.5 - két diszkrét esetén? a sorrend számít
.then(chart => 
  chart.animate({ 
    config: {
      channels: {
        x: { set: ['Types','Genres'] },
      },
      legend: 'color',
      title: '...grouping is determined by their order.'
    }
    }))

.


        catch((err) => {
          console.log(err);
        });

 

