<p align="center">
  <a href="https://lib.vizzuhq.com/latest/">
    <img src="https://lib.vizzuhq.com/latest/readme/infinite-60.gif" alt="Vizzu" />
  </a>
  <p align="center"><b>Vizzu</b> - Library for animated data visualizations and data stories.</p>
  <p align="center">
    <a href="https://lib.vizzuhq.com/latest/">Documentation</a>
    · <a href="https://lib.vizzuhq.com/latest/examples/">Examples</a>
    · <a href="https://lib.vizzuhq.com/latest/reference/">Code reference</a>
    · <a href="https://github.com/vizzuhq/vizzu-lib">Repository</a>
    · <a href="https://blog.vizzuhq.com">Blog</a>
  </p>
</p>

[![npm version](https://badge.fury.io/js/vizzu.svg)](https://badge.fury.io/js/vizzu)
[![install size](https://packagephobia.com/badge?p=vizzu)](https://packagephobia.com/result?p=vizzu)
[![Tweet](https://img.shields.io/twitter/url/http/shields.io.svg?style=social)](https://twitter.com/intent/tweet?text=Vizzu%3A%20an%20open-source%20library%20for%20animated%20data%20visualizations%20and%20data%20stories&url=https://github.com/vizzuhq/vizzu-lib&via=vizzuhq&hashtags=vizzu,dataviz,javascript,opensource,developers)

# Vizzu

## About The Project

Vizzu is a free, open-source Javascript/C++ library utilizing a generic dataviz
engine that generates many types of charts and seamlessly animates between them.
It can be used to create static charts but more importantly, it is designed for
building animated data stories and interactive explorers as Vizzu enables
showing different perspectives of the data that the viewers can easily follow
due to the animation.

Main features:

- Designed with animation in focus;
- Defaults based on data visualization guidelines;
- Automatic data aggregation & data filtering;
- HTML5 canvas rendering;
- Written in C++ compiled to WebAssembly;
- Dependency-free.

## Installation

Install via [npm](https://www.npmjs.com/package/vizzu):

```sh
npm install vizzu
```

Or use it from CDN:

```html
<html>
 <head>
  <script type="module">
   import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@latest/dist/vizzu.min.js';
  </script>
 </head>
</html>

```

## Usage

Create a placeholder element that will contain the rendered chart:

```html
<html>
 <body>
  <div id="myVizzu" style="width:800px; height:480px;">
  </div>
 </body>
</html>

```

Create a simple bar chart:

```javascript
import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@latest/dist/vizzu.min.js';

let data = {
    series: [{
        name: 'Foo',
        values: ['Alice', 'Bob', 'Ted']
    }, {
        name: 'Bar',
        values: [15, 32, 12]
    }, {
        name: 'Baz',
        values: [5, 3, 2]
    }]
};

let chart = new Vizzu('myVizzu', {
    data
});
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

[Try it!](https://jsfiddle.net/VizzuHQ/dk7b86vc)

![Example chart](https://lib.vizzuhq.com/latest/readme/example.gif)

## FAQ

You can find answers to the most frequently asked questions about using the
library in our [FAQ](https://lib.vizzuhq.com/latest/FAQ/).

## Projects

List of external projects (extensions, bindings, templates, etc) for Vizzu:
[Projects](https://lib.vizzuhq.com/latest/PROJECTS/).

## Roadmap

We have a comprehensive list of features we plan to implement, on our
[Roadmap](https://github.com/vizzuhq/.github/wiki/Roadmap).

## Contributing

We welcome contributions to the project, visit our contributing
[guide](https://lib.vizzuhq.com/latest/CONTRIBUTING/) for further info.

## Contact

- Join our Slack:
  [vizzu-community.slack.com](https://join.slack.com/t/vizzu-community/shared_invite/zt-w2nqhq44-2CCWL4o7qn2Ns1EFSf9kEg)
- Drop us a line at hello@vizzu.io
- Follow us on Twitter:
  [https://twitter.com/VizzuHQ](https://twitter.com/VizzuHQ)

## License

Copyright © 2021-2025 [Vizzu Inc.](https://vizzuhq.com)

Released under the
[Apache 2.0 License](https://lib.vizzuhq.com/latest/LICENSE/).
