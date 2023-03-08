---
data_url: ../../assets/data/music_data.js
---

# Sorting

`Vizzu` provides multiple options to sort data. By default, the data is sorted
by the order it is added to `Vizzu`. This is why we suggest to add temporal data
such as dates in chronological order - from oldest to newest.

You can also sort the elements by value, which will provide you with an
ascending order.

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
                    set: ['Popularity', 'Kinds']
                },
                x: {
                    set: ['Genres']
                },
                label: {
                    attach: ['Popularity']
                },
            },
            color: {
                attach: ['Kinds']
            },
        }
    })
    ```

```javascript
chart.animate({
    config: {
        sort: 'byValue'
    }
})
```

If you want descending order instead, you have to set the `reverse` parameter to
`true`. When used without setting the sorting to `byValue`, the elements will be
in the opposite order than they are in the data set added to `Vizzu`.

<div id="tutorial_02"></div>

```javascript
chart.animate({
    config: {
        reverse: true
    }
})
```

This is how to switch back to the default sorting.

<div id="tutorial_03"></div>

```javascript
chart.animate({
    config: {
        sort: 'none',
        reverse: false
    }
})
```

When you have more than one dimension on a channel, their order determines how
the elements are grouped. For example, below - each set of bars is first
organized by `Genres`, and then we have one bar for each of `Kinds`.

<div id="tutorial_04"></div>

```javascript
chart.animate({
    config: {
        channels: {
            y: {
                detach: ['Kinds']
            },
            x: {
                set: ['Genres', 'Kinds']
            }
        }
    }
})
```

When switching the order of dimensions on the x-axis `Vizzu` will rearrange the
elements according to this new logic.

!!! note
    The legend is automatically removed during the animation.

<div id="tutorial_05"></div>

```javascript
chart.animate({
    config: {
        channels: {
            x: {
                set: ['Kinds', 'Genres']
            },
        }
    }
})
```

<script src="../sorting.js"></script>
