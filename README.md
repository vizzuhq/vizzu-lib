# About The Project

Vizzu is a free, open-source Javascript and C++ library utilizing a dataviz engine that generates all types of charts and seamlessly animates between them. It's an excellent choice for building animated data stories and interactive explorers as Vizzu enables showing different perspectives of the data that the viewers can easily follow due to the animation.

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
    { name: 'Foo', type: 'categories', values: ['A', 'B', 'C'] },
    { name: 'Bar', type: 'values', values: [15, 32, 12] },
    { name: 'Baz', type: 'values', values: [15, 32, 12] }
  ]
};

let chart = new Vizzu("myVizzu");

let animation = chart.initializing.then(chart => chart.animate(
  {
    data,    
    config: {
      channels: {
        x: { attach: ['Foo'] },
        y: { attach: ['Bar']},
      }
    }
  }
));
```

Then turn it into a scatter plot:

```javascript
animation.then(chart => chart.animate(
  {
    config: {
      channels: {
        color: { attach: ['Foo'] }, 
        x: { detach: ['Foo'], attach: ['Baz'] }
      },
      geometry: 'circle'
    }
  }
));
```

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
