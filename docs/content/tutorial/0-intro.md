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
import Vizzu from 
  'https://cdn.jsdelivr.net/npm/vizzu@~0.5.0/dist/vizzu.min.js';
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

### Working with bundlers & frameworks

The published Vizzu package contains a Javascript (*vizzu.min.js*) and a 
WebAssembly (*cvizzu.wasm*) module. By default, the Javascript module tries to load the wasm module from the same path it was imported from. 

This behavior won't work with bundlers if they move the wasm file to a different folder. To handle this issue, you can manually set the wasm module's location.

```Javascript { "run": false }
Vizzu.options({ wasmUrl: "/path/to/cvizzu.wasm" })
```

> Note: you should set this path before the Vizzu constructor is called because it loads the wasm module.

Different bundlers will need additional, specific settings to recognize the wasm file as a dependency and handle it correctly. For setting up Vizzu with a specific bundler or framework, check the corresponding how-to/example below:

- Webpack: https://github.com/vizzuhq/vizzu-webpack-demo
- React: https://vizzuhq.github.io/vizzu-react-example/
- Vite: https://github.com/simzer/vizzu-vite-demo

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

The parameters of multiple animate methods can be bundled together and can be
passed to the animate method at once, resulting in these animations running one 
after the other. See Keyframe objects in [Animation control & keyframes](#chapter-0.15).
