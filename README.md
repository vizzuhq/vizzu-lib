<p align="center">
  <a href="https://github.com/vizzuhq/vizzu-lib">
    <img src="/docs/images/logo-bg-white.svg" alt="Vizzu" />
  </a>
  <p align="center">Library for animated data visualizations and data stories.</p>
  <p align="center">
    Tutorial 
    · How-tos 
    · <a href="https://vizzuhq.github.io/vizzu-lib-doc/0.3.0/reference/">Reference</a>
  </p>
</p>

# About The Project

Vizzu is a free, open-source Javascript/C++ library utilizing a generic dataviz engine 
that generates many types of charts and seamlessly animates between them. 
It is designed for building animated data stories and interactive explorers 
as Vizzu enables showing different perspectives of the data that the viewers can 
easily follow due to the animation.

Main features:
- Designed with animation in focus; 
- Defaults based on data visualization guidelines;
- Automatic data aggregation, data filtering;
- HTML5 canvas rendering;
- Written in C++ compiled to WebAssembly;
- Dependency-free.

# Usage

Create a placeholder element that will contain the rendered chart:

```html
<div id="myVizzu" style="width:800px; height:480px;"></div>
```

Create a simple bar chart:

```javascript
import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';

let data = {
  series: [
    { name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
    { name: 'Bar', values: [15, 32, 12] },
    { name: 'Baz', values: [5, 3, 2] }
  ]
};

let chart = new Vizzu('myVizzu', { data });
```

```javascript
chart.animate({
  x: 'Foo',
  y: 'Bar'
});
```

Then turn it into a scatter plot:

```javascript
chart.animate({
  color: 'Foo',
  x: 'Baz', 
  geometry: 'circle' 
});
```
[Try it!](https://jsfiddle.net/VizzuHQ/dk7b86vc/19/)

![Example chart](https://vizzuhq.github.io/vizzu-lib-doc/readme/example.gif)

# Releases

## Nightly builds 

URL of the build from the HEAD commit of the Main branch:

* https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js

URLs of all builds on Main branch of the past 30 days:

* https://vizzu-lib-main-sha.storage.googleapis.com/lib-HASH/vizzu.js

where HASH is the first 7 character of the commit's git hash.

# License

Copyright © 2021 [Vizzu Kft.](https://vizzuhq.com).

Released under the [Apache 2.0 License](LICENSE).

# Building 

[Building](project/build.md)

# Testing

[Testing](test/test.md)
