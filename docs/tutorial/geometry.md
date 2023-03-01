---
data_url: ../../assets/data/music_data.js
---

# Geometry

In `Vizzu` you can set the geometric elements used to represent your data by the
geometry property within the config object. This is where the library shines -
beautifully animating between the geometries!

Switching the geometry to area.

<div id="tutorial_01"></div>

??? info "Info - How to setup Vizzu"
    In `HTML`, create a placeholder element that will contain the rendered
    chart.

    ```html
    <html>
     <body>
      <div id="myVizzu">
      </div>
     </body>
    </html>

    ```

    In `JavaScript`, initialize and configure the chart:

    ```javascript
    import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@latest/dist/vizzu.min.js'
    import data from 'https://lib.vizzuhq.com/latest/assets/data/music_data.js'

    let chart = new Vizzu('myVizzu')

    chart.initializing

    chart.animate({
        data: data,
        config: {
            channels: {
                y: {
                    set: ['Popularity']
                },
                x: {
                    set: ['Genres']
                },
            },
        }
    })
    ```

```javascript
chart.animate({
    config: {
        geometry: 'area',
    }
})
```

Drawing a line chart.

<div id="tutorial_02"></div>

```javascript
chart.animate({
    config: {
        geometry: 'line',
    }
})
```

Switching the geometry to circle. This setting is the most useful when used
together with the size channel, as shown in the next chapter of the tutorial.

<div id="tutorial_03"></div>

```javascript
chart.animate({
    config: {
        geometry: 'circle',
    }
})
```

Rectangle geometry is the default setting in `Vizzu`, used for most common
charts like bar and column charts.

<div id="tutorial_04"></div>

```javascript
chart.animate({
    config: {
        geometry: 'rectangle',
    }
})
```

<script src="../geometry.js"></script>
