<p align="center">
  <a href="https://github.com/vizzuhq/vizzu-lib">
    <img src="https://vizzuhq.github.io/vizzu-lib-doc/readme/logo-bg-white.svg" alt="Vizzu" />
  </a>
  <p align="center">Library for animated data visualizations and data stories.</p>
  <p align="center">
    <a href="https://lib.vizzuhq.com/0.3.0/">Tutorial & Examples</a>
    · <a href="https://lib.vizzuhq.com/0.3.0/reference">Reference</a> 
    · <a href="https://github.com/vizzuhq/vizzu-lib">Repository</a>
  </p>
</p>

# About The Project

Vizzu is a free, open-source Javascript/C++ library utilizing a generic dataviz engine 
that generates many types of charts and seamlessly animates between them. 
It can be used to create static charts but more importantly it is designed for building animated data stories 
and interactive explorers as Vizzu enables showing different perspectives of the data that the viewers can 
easily follow due to the animation.

Main features:
- Designed with animation in focus; 
- Defaults based on data visualization guidelines;
- Automatic data aggregation & data filtering;
- HTML5 canvas rendering;
- Written in C++ compiled to WebAssembly;
- Dependency-free.

# Installation

Install via [npm](https://www.npmjs.com/package/vizzu):

    npm install vizzu

Or use it from CDN:

```html
<script type="module">
import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@0.3.0/dist/vizzu.min.js';
</script>
```

# Usage

Create a placeholder element that will contain the rendered chart:

```html
<div id="myVizzu" style="width:800px; height:480px;"></div>
```

Create a simple bar chart:

```javascript
import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@0.3.0/dist/vizzu.min.js';

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
[Try it!](https://jsfiddle.net/VizzuHQ/dk7b86vc/29/)

![Example chart](https://vizzuhq.github.io/vizzu-lib-doc/readme/example.gif)

# Contributing

We welcome contributions to the project, visit our [Contributing](CONTRIBUTING.md) page for further info.

# Contact

* Join our Slack: [vizzu-community.slack.com](https://join.slack.com/t/vizzu-community/shared_invite/zt-w2nqhq44-2CCWL4o7qn2Ns1EFSf9kEg)
* Drop us a line at hello@vizzuhq.com
* Follow us on twitter: [https://twitter.com/VizzuHQ](https://twitter.com/VizzuHQ)

# License

Copyright © 2021 [Vizzu Kft.](https://vizzuhq.com).

Released under the [Apache 2.0 License](LICENSE).
