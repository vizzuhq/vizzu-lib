## Intro

This is the tutorial of Vizzu - a free, open-source Javascript library for 
building animated charts, data stories, and interactive explorers. 
It is an excellent place to start using Vizzu, as it walks you through the 
installation and initialization of the library, introduces the logic it employs 
and the different settings to control how your charts look and behave. 

Starting from the [Axes, title, tooltip](#chapter-0.2) chapter, you can see code 
snippets, and when you click on them, you can see how the code affects the chart 
on the right side of the screen.

### Installation

Import the library into a JavaScript file or script tag in your web page

```html
<script type="module">
import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@0.3.1/dist/vizzu.min.js';
</script>
```

Alternatively, you can install Vizzu from [npm](https://www.npmjs.com/package/vizzu)

```shell
npm install vizzu
```

### Initialization

To show a Vizzu chart create a placeholder element that will contain the 
rendered chart.

```html
<div id="myVizzu" style="width:800px; height:480px;"></div>
```

Create a new object instance of the Vizzu class, add it to a variable, 
initialize the library, and then you can start setting up the first chart by 
calling the animate method.

```javascript { "run": false }
let chart = new Vizzu('myVizzu')

chart.initializing.then(
  chart => chart.animate({ /* ... */ }) 
)
```

### The basic logic of Vizzu

The foundation of a Vizzu chart is the animation. The animation contains states describing the chart's configuration, such as the data series, coordinate system, labels, titles, etc. A static chart is the result of a single animation state. When there are more states, Vizzu automatically transitions between these. The animate method initiates the animation into a new state by describing the new chart and how Vizzu should transition to it. The return value of the animate method is a promise that will be resolved after the animation is completed. Using this logic you can create a promise chain of animation from state to state.

<img src="images/api-overview.svg" width="100%" alt="Illustration of Vizzu's promise chain">

The animate method has two parameters in total. The first parameter sets the 
chart, and the (optional) second determines how Vizzu should animate to that state. 

The first parameter has the following three properties:
- *data* - this is where you add the data that you want to put on the charts.
- *config* - this is where you can add or remove series on the channels and 
  set the general settings of the chart like the chart title, the geometry, the alignment etc.
- *style* - this is where you can set how your chart looks.
