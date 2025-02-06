# Tutorial

This is the tutorial of `Vizzu` - a free, open-source `JavaScript` library for
building animated charts, data stories, and interactive explorers. It is an
excellent place to start using `Vizzu`, as it walks you through the installation
and initialization of the library, introduces the logic it employs and the
different settings to control how your charts look and behave.

## Basic logic of Vizzu

The foundation of a `Vizzu` chart is the animation. The animation contains
states describing the chart's configuration, such as the data series, coordinate
system, labels, titles, etc. A static chart is the result of a single animation
state. When there are more states, `Vizzu` automatically transitions between
these. The animate method initiates the animation into a new state by describing
the new chart and how `Vizzu` should transition to it. The return value of the
animate method is a promise that will be resolved after the animation is
completed. Using this logic you can create a promise chain of animation from
state to state.

![Vizzu](../assets/api-overview.svg){ class='image-center' }

The animate method has two parameters in total. The first parameter sets the
chart, and the (optional) second determines how `Vizzu` should animate to that
state.

The first parameter has the following three properties:

- `data`: this is where you add the data that you want to put on the charts
- `config`: this is where you can add or remove series on the channels and set
    the general settings of the chart like the chart title, the geometry, the
    alignment etc.
- `style`: this is where you can set how your chart looks

The parameters of multiple animate methods can be bundled together and can be
passed to the animate method at once, resulting in these animations running one
after the other. See `Keyframe` objects in
[Animation control & keyframes](./animation_control_keyframes.md).

## Installation

Install via [npm](https://www.npmjs.com/package/vizzu):

```sh
npm install vizzu
```

Or use it from [CDN](https://www.jsdelivr.com/package/npm/vizzu):

```html
<html>
 <head>
  <script type="module">
   import Vizzu from 
  'https://cdn.jsdelivr.net/npm/vizzu@xLIB_MINOR_VERSIONx/dist/vizzu.min.js';
  </script>
 </head>
</html>
```

## Usage
