---
data_url: ../../assets/data/music_data.js
---

# Animation control & keyframes

Using the result object provided by the chart's animate method you can play,
pause, stop, seek or reverse the animations.

In this step, we seek forward to `50%` of progress after the animation starts.

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
        channels: {
            x: {
                attach: ['Kinds']
            },
            y: {
                detach: ['Kinds']
            }
        },
    }
}).activated.then(control => control.seek('50%'));
```

You can also control the initial position and play state of the animation
through the animation options argument of the animate method.

<div id="tutorial_02"></div>

```javascript
chart.animate({
    config: {
        channels: {
            x: {
                detach: ['Kinds']
            },
            y: {
                attach: ['Kinds']
            }
        },
    }
}, {
    playState: 'paused',
    position: 0.5
}).activated.then(control => control.play());
```

You may want to control multiple animations as a single one. For example you
might want to go back to the original state if any of the animations get
canceled, or want to seamlessly seek through all of them.

You can do this by boundling them together and passing them to a single
`animate` call. To do this, you need to create a `Keyframe` object from the
arguments of every single `animate` call and then puting them into an array of
keyframes.

<div id="tutorial_03"></div>

```javascript
chart.animate([{
    target: {
        config: {
            channels: {
                x: {
                    attach: ['Kinds']
                },
                y: {
                    detach: ['Kinds']
                }
            },
        }
    },
    options: {
        duration: 0.5
    }
}, {
    target: {
        config: {
            channels: {
                x: {
                    detach: ['Kinds']
                },
                y: {
                    attach: ['Kinds']
                }
            }
        }
    },
    options: {
        duration: 1
    }
}]);
```

The initial state of the animation can be set, too, by using the second argument
of the `animate` method.

<script src="../animation_control_keyframes.js"></script>
